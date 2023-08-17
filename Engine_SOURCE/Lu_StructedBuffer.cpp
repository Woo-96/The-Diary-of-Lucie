#include "Lu_StructedBuffer.h"
#include "Lu_GraphicDevice_Dx11.h"

namespace Lu::graphics
{
	StructedBuffer::StructedBuffer()
		: GpuBuffer()
		, m_SRV(nullptr)
		, m_UAV(nullptr)
		, m_Type(eViewType::SRV)
		, m_Size(0)
		, m_Stride(0)
		, m_SRVSlot(0)
		, m_UAVslot(0)
	{

	}

	StructedBuffer::~StructedBuffer()
	{

	}

    bool StructedBuffer::Create(UINT _Size, UINT _Stride, eViewType _Type, void* _Data, bool _bCPUAccess)
    {
        m_Type = _Type;

        m_Size = _Size;
        m_Stride = _Stride;

        Desc.ByteWidth = m_Size * m_Stride;
        Desc.StructureByteStride = m_Size;

        Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        Desc.CPUAccessFlags = 0;
        Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;	// Texture Register Binding
        Desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 구조화 버퍼 추가 플래그 설정

        if (m_Type == eViewType::UAV)
        {
            Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
            Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE
                | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;	// Texture Register Binding
            Desc.CPUAccessFlags = 0;
        }

        if (_Data)
        {
            D3D11_SUBRESOURCE_DATA tSub = {};
            tSub.pSysMem = _Data;

            if (!(GetDevice()->CreateBuffer(Buffer.GetAddressOf(), &Desc, &tSub)))
                return false;
        }
        else
        {
            if (!(GetDevice()->CreateBuffer(Buffer.GetAddressOf(), &Desc, nullptr)))
                return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.BufferEx.NumElements = m_Stride;
        srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;

        if (!(GetDevice()->CreateShaderResourceView(Buffer.Get(), &srvDesc, m_SRV.GetAddressOf())))
            return false;

        if (m_Type == eViewType::UAV)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Buffer.NumElements = m_Stride;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

            if (!GetDevice()->CreateUnordedAccessView(Buffer.Get(), &uavDesc, m_UAV.GetAddressOf()))
                return false;
        }

        if (_bCPUAccess)
            CreateRWBuffer();

        return true;
    }

    bool StructedBuffer::CreateRWBuffer()
    {
        D3D11_BUFFER_DESC wDesc(Desc);

        wDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 구조화 버퍼 추가 플래그 설정
        wDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Texture Register Binding	

        wDesc.Usage = D3D11_USAGE_DYNAMIC;
        wDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        if (!GetDevice()->CreateBuffer(m_WriteBuffer.GetAddressOf(), &wDesc, nullptr))
            return false;

        D3D11_BUFFER_DESC rDesc(Desc);

        rDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 구조화 버퍼 추가 플래그 설정
        rDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Texture Register Binding	

        rDesc.Usage = D3D11_USAGE_DEFAULT;
        rDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

        if (!GetDevice()->CreateBuffer(m_ReadBuffer.GetAddressOf(), &rDesc, nullptr))
            return false;

        return true;
    }

    void StructedBuffer::SetData(void* _Data, UINT _Stride)
	{
        if (m_Stride < _Stride)
            Create(m_Size, _Stride, m_Type, _Data);
        else
            GetDevice()->BindBuffer(m_WriteBuffer.Get(), _Data, m_Size * _Stride);

        GetDevice()->CopyResource(Buffer.Get(), m_WriteBuffer.Get());
	}

    void StructedBuffer::GetData(void* _Data, UINT _Size)
    {
        GetDevice()->CopyResource(m_ReadBuffer.Get(), Buffer.Get());

        if (_Size == 0)
            GetDevice()->BindBuffer(m_ReadBuffer.Get(), _Data, m_Size * m_Stride);
        else
            GetDevice()->BindBuffer(m_ReadBuffer.Get(), _Data, _Size);
    }

	void StructedBuffer::BindSRV(eShaderStage _Stage, UINT _Slot)
	{
        m_SRVSlot = _Slot;
        GetDevice()->BindShaderResource(_Stage, _Slot, m_SRV.GetAddressOf());
	}

	void StructedBuffer::BindUAV(UINT _Slot)
	{
        m_UAVslot = _Slot;
        UINT i = -1;
        GetDevice()->BindUnorderedAccess(_Slot, m_UAV.GetAddressOf(), &i);
	}

	void StructedBuffer::Clear()
	{
        // SRV Clear
        ID3D11ShaderResourceView* srv = nullptr;
        GetDevice()->BindShaderResource(eShaderStage::VS, m_SRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::HS, m_SRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::DS, m_SRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::GS, m_SRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::PS, m_SRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::CS, m_SRVSlot, &srv);

        // UAV Clear
        ID3D11UnorderedAccessView* uav = nullptr;
        UINT i = -1;
        GetDevice()->BindUnorderedAccess(m_UAVslot, &uav, &i);
	}
}
#include "Lu_ConstantBuffer.h"
#include "Lu_GraphicDevice_Dx11.h"

namespace Lu::graphics
{
	ConstantBuffer::ConstantBuffer(const eCBType type)
		: GpuBuffer()
		, m_Type(type)
	{

	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(size_t size)
	{
		Desc.ByteWidth = (UINT)size;
		Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		Desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Lu::graphics::GetDevice()->CreateBuffer(Buffer.GetAddressOf(), &Desc, nullptr);

		return false;
	}

	void ConstantBuffer::SetData(void* data)
	{
		Lu::graphics::GetDevice()->SetConstantBuffer(Buffer.Get(), data, Desc.ByteWidth);
	}

	void ConstantBuffer::Bind(eShaderStage stage)
	{
		Lu::graphics::GetDevice()->BindConstantBuffer(stage, m_Type, Buffer.Get());
	}
}
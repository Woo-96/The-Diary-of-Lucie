#include "Lu_Texture.h"
#include "Lu_GraphicDevice_Dx11.h"

namespace Lu::graphics
{
	Texture::Texture()
		: Resource(enums::eResourceType::Texture)
		, m_Image{}
		, m_Texture(nullptr)
		, m_SRV(nullptr)
		, m_Width(0)
		, m_Height(0)
		, m_Desc{}
	{
	}

	Texture::~Texture()
	{
	}

	bool Texture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag)
	{
		if (m_Texture == nullptr)
		{
			m_Desc.BindFlags = _BindFlag;
			m_Desc.Usage = D3D11_USAGE_DEFAULT;
			m_Desc.CPUAccessFlags = 0;
			m_Desc.Format = _Format;
			m_Desc.Width = _Width;
			m_Desc.Height = _Height;
			m_Desc.ArraySize = 1;
			 
			m_Desc.SampleDesc.Count = 1;
			m_Desc.SampleDesc.Quality = 0;
			 
			m_Desc.MipLevels = 0;
			m_Desc.MiscFlags = 0;
			 
			m_Width = _Width;
			m_Height = _Height;

			if (!GetDevice()->CreateTexture2D(&m_Desc, nullptr, m_Texture.GetAddressOf()))
				return false;
		}

		if (_BindFlag & (UINT)D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (!GetDevice()->CraeteDepthStencilView(m_Texture.Get(), nullptr, m_DSV.GetAddressOf()))
				return false;
		}
		if (_BindFlag & (UINT)D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = m_Desc.Format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

			if (!GetDevice()->CreateShaderResourceView(m_Texture.Get(), &tSRVDesc, m_SRV.GetAddressOf()))
				return false;
		}

		if (_BindFlag & (UINT)D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			D3D11_RENDER_TARGET_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = m_Desc.Format;
			tSRVDesc.Texture2D.MipSlice = 0;
			tSRVDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;

			if (!GetDevice()->CreateRenderTargetView(m_Texture.Get(), nullptr, m_RTV.GetAddressOf()))
				return false;
		}

		if (_BindFlag & (UINT)D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = m_Desc.Format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			if (!GetDevice()->CreateUnordedAccessView(m_Texture.Get(), &tUAVDesc, m_UAV.GetAddressOf()))
				return false;
		}

		return true;
	}

	void Texture::BindShaderResource(eShaderStage _Stage, UINT _StartSlot)
	{
		GetDevice()->BindShaderResource(_Stage, _StartSlot, m_SRV.GetAddressOf());
	}

	void Texture::BindUnorderedAccessViews(UINT _Slot)
	{
		UINT i = -1;
		GetDevice()->BindUnorderedAccess(_Slot, m_UAV.GetAddressOf(), &i);
	}

	void Texture::ClearUnorderedAccessViews(UINT _Slot)
	{
		ID3D11UnorderedAccessView* p = nullptr;
		UINT i = -1;
		GetDevice()->BindUnorderedAccess(_Slot, &p, &i);
	}

	void Texture::Clear()
	{
		ID3D11ShaderResourceView* srv = nullptr;

		GetDevice()->BindShaderResource(eShaderStage::VS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::DS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::GS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::HS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::CS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::PS, 0, &srv);
	}

	HRESULT Texture::Load(const std::wstring& _Path)
	{
		wchar_t szExtension[50] = {};
		_wsplitpath_s(_Path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 50);

		std::wstring extension = szExtension;
		if (extension == L".dds" || extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(_Path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image)))
				return S_FALSE;
		}
		else if (extension == L".tga" || extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(_Path.c_str(), nullptr, m_Image)))
				return S_FALSE;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(_Path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image)))
				return S_FALSE;
		}

		CreateShaderResourceView
		(
			GetDevice()->GetID3D11Device()
			, m_Image.GetImages()
			, m_Image.GetImageCount()
			, m_Image.GetMetadata()
			, m_SRV.GetAddressOf()
		);
		m_SRV->GetResource((ID3D11Resource**)m_Texture.GetAddressOf());

		m_Width = (UINT)m_Image.GetMetadata().width;
		m_Height = (UINT)m_Image.GetMetadata().height;

		return S_OK;
	}
}
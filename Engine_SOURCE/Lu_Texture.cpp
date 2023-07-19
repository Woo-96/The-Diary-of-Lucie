#include "Lu_Texture.h"
#include "Lu_GraphicDevice_Dx11.h"

namespace Lu::graphics
{
	Texture::Texture()
		: Resource(enums::eResourceType::Texture)
		, m_Image{}
		, m_Texture(nullptr)
		, m_SRV(nullptr)
		, m_Desc{}
	{
	}

	Texture::~Texture()
	{
	}

	void Texture::BindShader(eShaderStage stage, UINT startSlot)
	{
		GetDevice()->BindShaderResource(stage, startSlot, m_SRV.GetAddressOf());
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

		return S_OK;
	}
}
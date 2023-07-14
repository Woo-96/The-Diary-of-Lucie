#include "Lu_Material.h"

namespace Lu::graphics
{
	Material::Material()
		: Resource(Lu::enums::eResourceType::Material)
		, m_Shader(nullptr)
		, m_Texture(nullptr)
		, m_Mode(eRenderingMode::Opaque)
	{

	}

	Material::~Material()
	{

	}

	void Material::Binds()
	{
		if (m_Texture)
			m_Texture->BindShader(eShaderStage::PS, 0);

		if (m_Shader)
			m_Shader->Binds();
	}

	void Material::Clear()
	{
		if (m_Texture)
			m_Texture->Clear();
	}

	HRESULT Material::Load(const std::wstring& _Path)
	{
		return E_NOTIMPL;
	}
}
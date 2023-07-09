#include "Lu_Material.h"

namespace Lu::graphics
{
	Material::Material()
		: Resource(Lu::enums::eResourceType::Material)
		, m_Shader(nullptr)
		, m_Texture(nullptr)
	{

	}

	Material::~Material()
	{

	}

	void Material::Binds()
	{
		m_Texture->BindShader(eShaderStage::PS, 0);
		m_Shader->Binds();
	}

	void Material::Clear()
	{
		m_Texture->Clear();
	}

	HRESULT Material::Load(const std::wstring& _Path)
	{
		return E_NOTIMPL;
	}
}
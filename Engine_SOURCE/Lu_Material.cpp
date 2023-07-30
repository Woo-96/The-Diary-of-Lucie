#include "Lu_Material.h"
#include "Lu_Renderer.h"

namespace Lu::graphics
{
	Material::Material()
		: Resource(Lu::enums::eResourceType::Material)
		, m_Shader(nullptr)
		, m_Texture(nullptr)
		, m_Mode(eRenderingMode::Opaque)
		, m_Const{}
	{

	}

	Material::~Material()
	{

	}

	void Material::SetScalarParam(SCALAR_PARAM _Param, const void* _Src)
	{
		switch (_Param)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
			m_Const.arrInt[(UINT)_Param - (UINT)INT_0] = *((int*)_Src);
			break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
			m_Const.arrFloat[(UINT)_Param - (UINT)FLOAT_0] = *((float*)_Src);
			break;

		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
			m_Const.arrV2[(UINT)_Param - (UINT)VEC2_0] = *((math::Vector2*)_Src);
			break;

		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
			m_Const.arrV4[(UINT)_Param - (UINT)VEC4_0] = *((math::Vector4*)_Src);
			break;
		}
	}

	void Material::GetScalarParam(SCALAR_PARAM _param, void* _pData)
	{
		switch (_param)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			*((int*)_pData) = m_Const.arrInt[(UINT)_param - (UINT)INT_0];
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			*((float*)_pData) = m_Const.arrFloat[(UINT)_param - (UINT)FLOAT_0];
		}
		break;

		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			*((math::Vector2*)_pData) = m_Const.arrV2[(UINT)_param - (UINT)VEC2_0];
		}
		break;

		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			*((math::Vector4*)_pData) = m_Const.arrV4[(UINT)_param - (UINT)VEC4_0];
		}
		break;
		}
	}

	void Material::Binds()
	{
		if (m_Texture)
			m_Texture->BindShader(eShaderStage::PS, 0);

		if (m_Shader)
			m_Shader->Binds();

		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Material];
		cb->SetData(&m_Const);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
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
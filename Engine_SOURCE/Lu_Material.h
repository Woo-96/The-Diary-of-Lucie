#pragma once
#include "Lu_Resource.h"
#include "Lu_Shader.h"
#include "Lu_Texture.h"

namespace Lu::graphics
{
	class Material : public Resource
	{
	public:
		Material();
		virtual ~Material();

	private:
		std::shared_ptr<Shader>		m_Shader;
		std::shared_ptr<Texture>	m_Texture;
		eRenderingMode				m_Mode;

		tMtrlConst                  m_Const;

	public:
		void SetShader(std::shared_ptr<Shader> _Shader)
		{ 
			m_Shader = _Shader;
		}

		void SetTexture(std::shared_ptr<Texture> _Texture)
		{ 
			m_Texture = _Texture;
		}

		void SetRenderingMode(eRenderingMode _Mode) 
		{ 
			m_Mode = _Mode; 
		}

		void SetScalarParam(SCALAR_PARAM _Param, const void* _Src);

	public:
		eRenderingMode GetRenderingMode() 
		{ 
			return m_Mode;
		}

		std::shared_ptr<Texture> GetTexture()
		{
			return m_Texture;
		}

		void GetScalarParam(SCALAR_PARAM _param, void* _pData);

	public:
		void Binds();
		void Clear();

	public:
		virtual HRESULT Load(const std::wstring& _Path) override;
	};
}
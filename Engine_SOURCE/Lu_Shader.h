#pragma once
#include "Lu_Resource.h"

#include "Lu_Resource.h"
#include "Lu_GraphicDevice_Dx11.h"

namespace Lu
{
	using namespace graphics;
	class Shader : public Resource
	{
	public:
		Shader();
		virtual ~Shader();

	private:
		ID3D11InputLayout*								m_InputLayout;
		D3D11_PRIMITIVE_TOPOLOGY						m_Topology;

		Microsoft::WRL::ComPtr<ID3DBlob>				m_VSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				m_HSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				m_DSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				m_GSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				m_PSBlob;

		Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_VS;
		Microsoft::WRL::ComPtr<ID3D11HullShader>		m_HS;
		Microsoft::WRL::ComPtr<ID3D11DomainShader>		m_DS;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_GS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_PS;

		eRSType											m_RSType;
		eDSType											m_DSType;
		eBSType											m_BSType;

	public:
		void SetRSState(eRSType _Type) 
		{ 
			m_RSType = _Type;
		}

		void SetDSState(eDSType _Type)
		{ 
			m_DSType = _Type;
		}

		void SetBSState(eBSType _Type)
		{ 
			m_BSType = _Type;
		}

		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology)
		{
			m_Topology = _Topology;
		}

	public:
		ID3DBlob* GetVSCode()
		{
			return m_VSBlob.Get();
		}

		ID3D11InputLayout* GetInputLayout()
		{
			return m_InputLayout;
		}

		ID3D11InputLayout** GetInputLayoutAddressOf()
		{
			return &m_InputLayout;
		}

	public:
		bool Create(const eShaderStage _Stage, const std::wstring& _FileName, const std::string& _FuncName);
		void Binds();

	public:
		virtual HRESULT Load(const std::wstring& _Path) override;
	};
}
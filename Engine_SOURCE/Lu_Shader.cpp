#include "Lu_Shader.h"
#include "Lu_Renderer.h"

namespace Lu
{
	Shader::Shader()
		: Resource(enums::eResourceType::Shader)
		, m_InputLayout(nullptr)
		, m_Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, m_RSType(eRSType::SolidNone)
		, m_DSType(eDSType::Less)
		, m_BSType(eBSType::AlphaBlend)
	{

	}

	Shader::~Shader()
	{
		m_InputLayout->Release();
	}

	bool Shader::Create(const eShaderStage _Stage
		, const std::wstring& _FileName
		, const std::string& _FuncName)
	{
		std::filesystem::path shaderPath
			= std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader_SOURCE\\";

		std::filesystem::path fullPath(shaderPath.c_str());
		fullPath += _FileName;

		ID3DBlob* errorBlob = nullptr;
		if (_Stage == eShaderStage::VS)
		{
			GetDevice()->CompileFromfile(fullPath, _FuncName, "vs_5_0", m_VSBlob.GetAddressOf());
			GetDevice()->CreateVertexShader(m_VSBlob->GetBufferPointer()
				, m_VSBlob->GetBufferSize(), m_VS.GetAddressOf());
		}
		else if (_Stage == eShaderStage::PS)
		{
			GetDevice()->CompileFromfile(fullPath, _FuncName, "ps_5_0", m_PSBlob.GetAddressOf());
			GetDevice()->CreatePixelShader(m_PSBlob->GetBufferPointer()
				, m_PSBlob->GetBufferSize(), m_PS.GetAddressOf());
		}

		return true;
	}

	void Shader::Binds()
	{
		GetDevice()->BindPrimitiveTopology(m_Topology);
		GetDevice()->BindInputLayout(m_InputLayout);

		GetDevice()->BindVertexShader(m_VS.Get());
		GetDevice()->BindPixelShader(m_PS.Get());

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rsState = renderer::rasterizerStates[(UINT)m_RSType];
		//Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState = renderer::depthStencilStates[(UINT)m_DSType];
		Microsoft::WRL::ComPtr<ID3D11BlendState> bsState = renderer::blendStates[(UINT)m_BSType];

		GetDevice()->BindRasterizeState(rsState.Get());
		//GetDevice()->BindDepthStencilState(dsState.Get());
		GetDevice()->BindBlendState(bsState.Get());
	}

	HRESULT Shader::Load(const std::wstring& _Path)
	{
		return E_NOTIMPL;
	}
}
#include "Lu_ComputeShader.h"
#include "Lu_GraphicDevice_Dx11.h"
#include "Lu_ConstantBuffer.h"
#include "Lu_Renderer.h"

namespace Lu::graphics
{
	ComputeShader::ComputeShader(int X, int Y, int Z)
		: Resource(enums::eResourceType::ComputeShader)
		, m_GroupX(1)
		, m_GroupY(1)
		, m_GroupZ(1)
		, m_Const{}
	{
		m_ThreadGroupCountX = X;
		m_ThreadGroupCountY = Y;
		m_ThreadGroupCountZ = Z;
	}

	ComputeShader::ComputeShader()
		: Resource(enums::eResourceType::ComputeShader)
		, m_GroupX(1)
		, m_GroupY(1)
		, m_GroupZ(1)
		, m_Const{}
	{
		m_ThreadGroupCountX = 32;
		m_ThreadGroupCountY = 32;
		m_ThreadGroupCountZ = 1;
	}

	ComputeShader::~ComputeShader()
	{
	}

	bool ComputeShader::Create(const std::wstring& _Name, const std::string& _MethodName)
	{
		std::filesystem::path shaderPath
			= std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader_SOURCE\\";

		std::filesystem::path fullPath(shaderPath.c_str());
		fullPath += _Name;

		ID3DBlob* errorBlob = nullptr;
		graphics::GetDevice()->CompileFromfile(fullPath, _MethodName, "cs_5_0", m_CSBlob.GetAddressOf());
		graphics::GetDevice()->CreateComputeShader(m_CSBlob->GetBufferPointer()
			, m_CSBlob->GetBufferSize(), m_CS.GetAddressOf());

		return true;
	}

	void ComputeShader::OnExcute()
	{
		Binds();

		// 자식 클래스에서 추가한 데이터(재질 상수 버퍼) 바인딩
		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Material];
		cb->SetData(&m_Const);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);

		GetDevice()->BindComputeShader(m_CS.Get());
		GetDevice()->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

		Clear();
	}
}
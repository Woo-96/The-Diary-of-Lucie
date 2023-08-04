#include "Lu_ComputeShader.h"
#include "Lu_GraphicDevice_Dx11.h"

namespace Lu::graphics
{
	ComputeShader::ComputeShader(int X, int Y, int Z)
		: Resource(enums::eResourceType::ComputeShader)
	{
		m_ThreadGroupCountX = X;
		m_ThreadGroupCountY = Y;
		m_ThreadGroupCountZ = Z;
	}

	ComputeShader::ComputeShader()
		: Resource(enums::eResourceType::ComputeShader)
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

		GetDevice()->BindComputeShader(m_CS.Get());
		GetDevice()->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

		Clear();
	}
}
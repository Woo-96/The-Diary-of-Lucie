#pragma once
#include "Lu_Resource.h"
#include "Lu_Graphics.h"

#include "../External/DirectXTex/Include/DirectXTex.h"
#include "../External/DirectXTex/Include/DirectXTex.inl"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Release\\DirectXTex.lib")
#endif

namespace Lu::graphics
{
	class Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

	private:
		ScratchImage										m_Image;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_Texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_SRV;
		D3D11_TEXTURE2D_DESC								m_Desc;

	public:
		size_t GetWidth() 
		{ 
			return m_Image.GetMetadata().width; 
		}

		size_t GetHeight() 
		{ 
			return m_Image.GetMetadata().height; 
		}

	public:
		void BindShader(eShaderStage _Stage, UINT _StartSlot);
		void Clear();

	public:
		virtual HRESULT Load(const std::wstring& _Path) override;
	};
}
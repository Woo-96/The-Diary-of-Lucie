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
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_RTV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_DSV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	m_UAV;

		D3D11_TEXTURE2D_DESC								m_Desc;

		UINT												m_Width;
		UINT												m_Height;

	public:
		void SetRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _RTV)
		{
			m_RTV = _RTV;
		}

		void SetDSV(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _DSV) 
		{ 
			m_DSV = _DSV;
		}

		void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture) 
		{ 
			m_Texture = _Texture;
		}

	public:
		size_t GetWidth()	const
		{ 
			return m_Width;
		}

		size_t GetHeight() const
		{ 
			return m_Height;
		}

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRTV() 
		{ 
			return  m_RTV; 
		}

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDSV()
		{
			return  m_DSV;
		}

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV() 
		{
			return  m_SRV; 
		}

		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GetUAV() 
		{ 
			return  m_UAV; 
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture()
		{ 
			return m_Texture;
		}


	public:
		bool Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag);
		void BindShaderResource(eShaderStage _Stage, UINT _StartSlot);
		void BindUnorderedAccessViews(UINT _Slot);
		void ClearUnorderedAccessViews(UINT _Slot);
		void Clear();

	public:
		virtual HRESULT Load(const std::wstring& _Path) override;
	};
}
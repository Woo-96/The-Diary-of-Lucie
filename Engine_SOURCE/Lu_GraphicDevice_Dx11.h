#pragma once
#include "Lucie_Engine.h"
#include "Lu_Graphics.h"
#include "Lu_Texture.h"

namespace Lu::graphics
{
	class GraphicDevice_Dx11
	{
	public:
		GraphicDevice_Dx11();
		~GraphicDevice_Dx11();

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>			m_Device;				// 실제 그래픽카드 하드웨어 객체
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_Context;				// 디바이스로 직접 명령X 컨텍스트를 통해서 명령O
		
		//Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_RenderTarget;			// 최종적으로 그려질 텍스처(도화지)
		//Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_RenderTargetView;		// 렌더 타겟에 직접 접근X 뷰를 통해서 접근O

		//Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_DepthStencilBuffer;	// 깊이 버퍼
		//Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;		// 깊이 타겟에 직접 접근X 뷰를 통해서 접근O

		std::shared_ptr<Lu::graphics::Texture>			m_RenderTarget;
		std::shared_ptr<Lu::graphics::Texture>			m_DepthStencil;

		Microsoft::WRL::ComPtr<IDXGISwapChain>			m_SwapChain;			// 더블버퍼링 작업을 진행해주는 swapChain

		D3D11_VIEWPORT									m_ViewPort;

	public:
		ID3D11Device* GetID3D11Device() 
		{ 
			return m_Device.Get();
		}

	public:
		bool CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* _Desc, HWND _hWnd);
		bool CreateTexture2D(const D3D11_TEXTURE2D_DESC* _Desc, void* _Data, ID3D11Texture2D** _ppTexture2D);
		bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* _pInputElementDescs, UINT _NumElements, ID3DBlob* _ByteCode, ID3D11InputLayout** _ppInputLayout);
		bool CreateBuffer(ID3D11Buffer** _Buffer, D3D11_BUFFER_DESC* _Desc, D3D11_SUBRESOURCE_DATA* _Data);
		bool CompileFromfile(const std::wstring& _FileName, const std::string& _FuncName, const std::string& _Version, ID3DBlob** _ppCode);
		bool CreateVertexShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11VertexShader** _ppVertexShader);
		bool CreateGeometryShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11GeometryShader** _ppGeometryShader);
		bool CreatePixelShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11PixelShader** _ppPixelShader);
		bool CreateComputeShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11ComputeShader** _ppComputeShader);
		bool CreateSampler(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState);
		bool CreateRasterizeState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState);
		bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState);
		bool CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState);
		bool CraeteDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView);
		bool CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRView);
		bool CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView);
		bool CreateUnordedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView);



		void BindInputLayout(ID3D11InputLayout* _pInputLayout);
		void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology);
		void BindVertexBuffer(UINT _StartSlot, ID3D11Buffer* const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets);
		void BindIndexBuffer(ID3D11Buffer* _pIndexBuffer, DXGI_FORMAT _Format, UINT _Offset);
		void BindVertexShader(ID3D11VertexShader* _pVetexShader);
		void BindGeometryShader(ID3D11GeometryShader* _pGeometryShader);
		void BindPixelShader(ID3D11PixelShader* _pPixelShader);
		void BindComputeShader(ID3D11ComputeShader* _pComputeShader);
		void Dispatch(UINT _ThreadGroupCountX, UINT _ThreadGroupCountY, UINT _ThreadGroupCountZ);

		void SetConstantBuffer(ID3D11Buffer* _Buffer, void* _Data, UINT _Size);
		void BindConstantBuffer(eShaderStage _Stage, eCBType _Type, ID3D11Buffer* _Buffer);
		void BindsConstantBuffer(eShaderStage _Stage, eCBType _Type, ID3D11Buffer* _Buffer);
		void BindBuffer(ID3D11Buffer* _Buffer, void* _Data, UINT _Size);
		void BindShaderResource(eShaderStage _Stage, UINT _StartSlot, ID3D11ShaderResourceView** _ppSRV);
		void BindUnorderedAccess(UINT _Slot, ID3D11UnorderedAccessView** _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts);
		
		void BindSampler(eShaderStage _Stage, UINT _StartSlot, ID3D11SamplerState** _ppSamplers);
		void BindViewPort(D3D11_VIEWPORT* _ViewPort);
		void BindRasterizeState(ID3D11RasterizerState* _pRasterizerState);
		void BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState);
		void BindBlendState(ID3D11BlendState* _pBlendState);


		void DrawIndexed(UINT _IndexCount, UINT _StartIndexLocation, INT _BaseVertexLocation);
		void DrawIndexedInstanced(UINT _IndexCountPerInstance, UINT _InstanceCount
			, UINT _StartIndexLocation, INT _BaseVertexLocation, UINT _StartInstanceLocation);
		void ClearTarget();
		void UpdateViewPort();
		void Draw();
		void Present();
	};

	inline GraphicDevice_Dx11*& GetDevice()
	{
		static GraphicDevice_Dx11* device = nullptr;
		return device;
	}
}

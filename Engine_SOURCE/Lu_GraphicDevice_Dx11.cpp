#include "Lu_GraphicDevice_Dx11.h"
#include "Lu_Application.h"
#include "Lu_Renderer.h"

extern Lu::Application application;

namespace Lu::graphics
{
	GraphicDevice_Dx11::GraphicDevice_Dx11()
	{
		/* 
		1. graphic Device, Context 생성
		2. SwapChain 생성
		3. RT, RT View 생성
		4. DS, DS View 생성
		5. RT Clear
		6. Present (RT에 있는 텍스쳐를 화면에 그리기)
		*/

		// Device, Context 생성
		HWND hWnd = application.GetHwnd();
		UINT deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel = (D3D_FEATURE_LEVEL)0;

		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
			, deviceFlag, nullptr, 0
			, D3D11_SDK_VERSION
			, m_Device.GetAddressOf(), &featureLevel
			, m_Context.GetAddressOf());

		// SwapChain 생성
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = application.GetWidth();
		swapChainDesc.BufferDesc.Height = application.GetHeight();

		if (!CreateSwapChain(&swapChainDesc, hWnd))
			return;

		// SwapChain으로부터 RT 얻어오기
		//if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
		//	, (void**)m_RenderTarget.GetAddressOf())))
		//	return;

		m_RenderTarget = std::make_shared<Texture>();
		m_DepthStencil = std::make_shared<Texture>();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget = nullptr;
		if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
			, (void**)renderTarget.GetAddressOf())))
			return;
		m_RenderTarget->SetTexture(renderTarget);

		// RT View 생성
		//m_Device->CreateRenderTargetView((ID3D11Resource*)m_RenderTarget.Get()
		//	, nullptr, m_RenderTargetView.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
		m_Device->CreateRenderTargetView((ID3D11Resource*)m_RenderTarget->GetTexture().Get()
			, nullptr, renderTargetView.GetAddressOf());
		m_RenderTarget->SetRTV(renderTargetView);


		// CreateTexture 함수에서 DS, DS View 생성
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.CPUAccessFlags = 0;

		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		depthStencilDesc.ArraySize = 1;

		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.MipLevels = 0;
		depthStencilDesc.MiscFlags = 0;

		//D3D11_SUBRESOURCE_DATA data;
		//if (!CreateTexture(&depthStencilDesc, &data))
		//	return;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
		if (!CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf()))
			return;
		m_DepthStencil->SetTexture(depthStencilBuffer);

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
		if (!CraeteDepthStencilView(depthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf()))
			return;
		m_DepthStencil->SetDSV(mDepthStencilView);


		RECT winRect = {};
		GetClientRect(hWnd, &winRect);

		m_ViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)
			, (float)(winRect.bottom - winRect.top)
			, 0.0f, 1.0f
		};

		BindViewPort(&m_ViewPort);

		//// OM Set : 출력 대상(RT) 설정
		//m_Context->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
		m_Context->OMSetRenderTargets(1, m_RenderTarget->GetRTV().GetAddressOf(), m_DepthStencil->GetDSV().Get());
}

	GraphicDevice_Dx11::~GraphicDevice_Dx11()
	{

	}

	bool GraphicDevice_Dx11::CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* _Desc, HWND _hWnd)
	{
		DXGI_SWAP_CHAIN_DESC dxgiDesc = {};

		dxgiDesc.OutputWindow = _hWnd;
		dxgiDesc.Windowed = true;
		dxgiDesc.BufferCount = _Desc->BufferCount;
		dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

		dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgiDesc.BufferDesc.Width = _Desc->BufferDesc.Width;
		dxgiDesc.BufferDesc.Height = _Desc->BufferDesc.Height;
		dxgiDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiDesc.BufferDesc.RefreshRate.Numerator = 240;
		dxgiDesc.BufferDesc.RefreshRate.Denominator = 1;
		dxgiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		dxgiDesc.SampleDesc.Count = 1;
		dxgiDesc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;

		if (FAILED(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return false;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return false;

		if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &dxgiDesc, m_SwapChain.GetAddressOf())))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateTexture2D(const D3D11_TEXTURE2D_DESC* _Desc, void* _Data, ID3D11Texture2D** _ppTexture2D)
	{
		//D3D11_TEXTURE2D_DESC dxgiDesc = {};
		//dxgiDesc.BindFlags = _Desc->BindFlags;
		//dxgiDesc.Usage = _Desc->Usage;
		//dxgiDesc.CPUAccessFlags = 0;

		//dxgiDesc.Format = _Desc->Format;
		//dxgiDesc.Width = _Desc->Width;
		//dxgiDesc.Height = _Desc->Height;
		//dxgiDesc.ArraySize = _Desc->ArraySize;

		//dxgiDesc.SampleDesc.Count = _Desc->SampleDesc.Count;
		//dxgiDesc.SampleDesc.Quality = 0;

		//dxgiDesc.MipLevels = _Desc->MipLevels;
		//dxgiDesc.MiscFlags = _Desc->MiscFlags;

		//if (FAILED(m_Device->CreateTexture2D(&dxgiDesc, nullptr, m_DepthStencilBuffer.ReleaseAndGetAddressOf())))
		//	return false;

		//if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf())))
		//	return false;

		//return true;

		if (FAILED(m_Device->CreateTexture2D(_Desc, nullptr, _ppTexture2D)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* _pInputElementDescs, UINT _NumElements, ID3DBlob* _ByteCode, ID3D11InputLayout** _ppInputLayout)
	{
		if (FAILED(m_Device->CreateInputLayout(_pInputElementDescs, _NumElements
			, _ByteCode->GetBufferPointer()
			, _ByteCode->GetBufferSize()
			, _ppInputLayout)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateBuffer(ID3D11Buffer** _Buffer, D3D11_BUFFER_DESC* _Desc, D3D11_SUBRESOURCE_DATA* _Data)
	{
		if (FAILED(m_Device->CreateBuffer(_Desc, _Data, _Buffer)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CompileFromfile(const std::wstring& _FileName, const std::string& _FuncName, const std::string& _Version, ID3DBlob** _ppCode)
	{
		ID3DBlob* errorBlob = nullptr;
		D3DCompileFromFile(_FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _FuncName.c_str(), _Version.c_str(), 0, 0, _ppCode, &errorBlob);

		if (errorBlob)
		{
			OutputDebugStringA((char*)(errorBlob->GetBufferPointer()));
			errorBlob->Release();
			errorBlob = nullptr;
		}

		return false;
	}

	bool GraphicDevice_Dx11::CreateVertexShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11VertexShader** _ppVertexShader)
	{
		if (FAILED(m_Device->CreateVertexShader(_pShaderBytecode, _BytecodeLength, nullptr, _ppVertexShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateGeometryShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11GeometryShader** _ppGeometryShader)
	{
		if (FAILED(m_Device->CreateGeometryShader(_pShaderBytecode, _BytecodeLength, nullptr, _ppGeometryShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreatePixelShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11PixelShader** _ppPixelShader)
	{
		if (FAILED(m_Device->CreatePixelShader(_pShaderBytecode, _BytecodeLength, nullptr, _ppPixelShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateComputeShader(const void* _pShaderBytecode, SIZE_T _BytecodeLength, ID3D11ComputeShader** _ppComputeShader)
	{
		if (FAILED(m_Device->CreateComputeShader(_pShaderBytecode, _BytecodeLength, nullptr, _ppComputeShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateSampler(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState)
	{
		if (FAILED(m_Device->CreateSamplerState(_pSamplerDesc, _ppSamplerState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateRasterizeState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState)
	{
		if (FAILED(m_Device->CreateRasterizerState(_pRasterizerDesc, _ppRasterizerState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState)
	{
		if (FAILED(m_Device->CreateDepthStencilState(_pDepthStencilDesc, _ppDepthStencilState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState)
	{
		if (FAILED(m_Device->CreateBlendState(_pBlendStateDesc, _ppBlendState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CraeteDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView)
	{
		if (FAILED(m_Device->CreateDepthStencilView(_pResource, _pDesc, _ppDepthStencilView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRView)
	{
		if (FAILED(m_Device->CreateShaderResourceView(_pResource, _pDesc, _ppSRView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView)
	{
		if (FAILED(m_Device->CreateRenderTargetView(_pResource, _pDesc, _ppRTView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateUnordedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView)
	{
		if (FAILED(m_Device->CreateUnorderedAccessView(_pResource, _pDesc, _ppUAView)))
			return false;

		return true;
	}

	void GraphicDevice_Dx11::BindInputLayout(ID3D11InputLayout* _pInputLayout)
	{
		m_Context->IASetInputLayout(_pInputLayout);
	}

	void GraphicDevice_Dx11::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology)
	{
		m_Context->IASetPrimitiveTopology(_Topology);
	}

	void GraphicDevice_Dx11::BindVertexBuffer(UINT _StartSlot, ID3D11Buffer* const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets)
	{
		m_Context->IASetVertexBuffers(_StartSlot, 1, _ppVertexBuffers, _pStrides, _pOffsets);
	}

	void GraphicDevice_Dx11::BindIndexBuffer(ID3D11Buffer* _pIndexBuffer, DXGI_FORMAT _Format, UINT _Offset)
	{
		m_Context->IASetIndexBuffer(_pIndexBuffer, _Format, _Offset);
	}

	void GraphicDevice_Dx11::BindVertexShader(ID3D11VertexShader* _pVetexShader)
	{
		m_Context->VSSetShader(_pVetexShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindGeometryShader(ID3D11GeometryShader* _pGeometryShader)
	{
		m_Context->GSSetShader(_pGeometryShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindPixelShader(ID3D11PixelShader* _pPixelShader)
	{
		m_Context->PSSetShader(_pPixelShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindComputeShader(ID3D11ComputeShader* _pComputeShader)
	{
		m_Context->CSSetShader(_pComputeShader, 0, 0);
	}

	void GraphicDevice_Dx11::Dispatch(UINT _ThreadGroupCountX, UINT _ThreadGroupCountY, UINT _ThreadGroupCountZ)
	{
		m_Context->Dispatch(_ThreadGroupCountX, _ThreadGroupCountY, _ThreadGroupCountZ);
	}

	void GraphicDevice_Dx11::SetConstantBuffer(ID3D11Buffer* _Buffer, void* _Data, UINT _Size)
	{
		D3D11_MAPPED_SUBRESOURCE subRes = {};
		m_Context->Map(_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
		memcpy(subRes.pData, _Data, _Size);
		m_Context->Unmap(_Buffer, 0);
	}

	void GraphicDevice_Dx11::BindConstantBuffer(eShaderStage _Stage, eCBType _Type, ID3D11Buffer* _Buffer)
	{
		switch (_Stage)
		{
		case eShaderStage::VS:
			m_Context->VSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
			break;
		case eShaderStage::HS:
			m_Context->HSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
			break;
		case eShaderStage::DS:
			m_Context->DSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
			break;
		case eShaderStage::GS:
			m_Context->GSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
			break;
		case eShaderStage::PS:
			m_Context->PSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
			break;
		case eShaderStage::CS:
			m_Context->CSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}
	void GraphicDevice_Dx11::BindsConstantBuffer(eShaderStage _Stage, eCBType _Type, ID3D11Buffer* _Buffer)
	{
		m_Context->VSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
		m_Context->HSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
		m_Context->DSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
		m_Context->GSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
		m_Context->PSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
		m_Context->CSSetConstantBuffers((UINT)_Type, 1, &_Buffer);
	}

	void GraphicDevice_Dx11::BindBuffer(ID3D11Buffer* _Buffer, void* _Data, UINT _Size)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		m_Context->Map(_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, _Data, _Size);
		m_Context->Unmap(_Buffer, 0);
	}

	void GraphicDevice_Dx11::BindShaderResource(eShaderStage _Stage, UINT _StartSlot, ID3D11ShaderResourceView** _ppSRV)
	{
		switch (_Stage)
		{
		case eShaderStage::VS:
			m_Context->VSSetShaderResources(_StartSlot, 1, _ppSRV);
			break;
		case eShaderStage::HS:
			m_Context->HSSetShaderResources(_StartSlot, 1, _ppSRV);
			break;
		case eShaderStage::DS:
			m_Context->DSSetShaderResources(_StartSlot, 1, _ppSRV);
			break;
		case eShaderStage::GS:
			m_Context->GSSetShaderResources(_StartSlot, 1, _ppSRV);
			break;
		case eShaderStage::PS:
			m_Context->PSSetShaderResources(_StartSlot, 1, _ppSRV);
			break;
		case eShaderStage::CS:
			m_Context->CSSetShaderResources(_StartSlot, 1, _ppSRV);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}

	void GraphicDevice_Dx11::BindUnorderedAccess(UINT _Slot, ID3D11UnorderedAccessView** _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts)
	{
		m_Context->CSSetUnorderedAccessViews(_Slot, 1, _ppUnorderedAccessViews, _pUAVInitialCounts);
	}

	void GraphicDevice_Dx11::BindSampler(eShaderStage _Stage, UINT _StartSlot, ID3D11SamplerState** _ppSamplers)
	{
		switch (_Stage)
		{
		case eShaderStage::VS:
			m_Context->VSSetSamplers(_StartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::HS:
			m_Context->HSSetSamplers(_StartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::DS:
			m_Context->DSSetSamplers(_StartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::GS:
			m_Context->GSSetSamplers(_StartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::PS:
			m_Context->PSSetSamplers(_StartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::CS:
			m_Context->CSSetSamplers(_StartSlot, 1, _ppSamplers);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}

	void GraphicDevice_Dx11::BindViewPort(D3D11_VIEWPORT* _ViewPort)
	{
		m_Context->RSSetViewports(1, _ViewPort);
	}

	void GraphicDevice_Dx11::BindRasterizeState(ID3D11RasterizerState* _pRasterizerState)
	{
		m_Context->RSSetState(_pRasterizerState);
	}

	void GraphicDevice_Dx11::BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState)
	{
		m_Context->OMSetDepthStencilState(_pDepthStencilState, 0);
	}

	void GraphicDevice_Dx11::BindBlendState(ID3D11BlendState* _pBlendState)
	{
		m_Context->OMSetBlendState(_pBlendState, nullptr, 0xffffffff);
	}

	void GraphicDevice_Dx11::CopyResource(ID3D11Resource* _pDstResource, ID3D11Resource* _pSrcResource)
	{
		m_Context->CopyResource(_pDstResource, _pSrcResource);
	}

	void GraphicDevice_Dx11::DrawIndexed(UINT _IndexCount, UINT _StartIndexLocation, INT _BaseVertexLocation)
	{
		m_Context->DrawIndexed(_IndexCount, _StartIndexLocation, _BaseVertexLocation);
	}

	void GraphicDevice_Dx11::DrawIndexedInstanced(UINT _IndexCountPerInstance, UINT _InstanceCount, UINT _StartIndexLocation, INT _BaseVertexLocation, UINT _StartInstanceLocation)
	{
		m_Context->DrawIndexedInstanced(_IndexCountPerInstance, _InstanceCount, _StartIndexLocation, _BaseVertexLocation, _StartInstanceLocation);
	}

	void GraphicDevice_Dx11::ClearTarget()
	{
		// render target clear
		//FLOAT bgColor[4] = { 0.f, 0.f, 0.f, 1.0f };
		//m_Context->ClearRenderTargetView(m_RenderTargetView.Get(), bgColor);
		//m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, (UINT8)0.0f);
		//m_Context->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		FLOAT bgColor[4] = { 0.f, 0.f, 0.f, 1.0f };
		m_Context->ClearRenderTargetView(m_RenderTarget->GetRTV().Get(), bgColor);
		m_Context->ClearDepthStencilView(m_DepthStencil->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, (UINT8)0.0f);
		m_Context->OMSetRenderTargets(1, m_RenderTarget->GetRTV().GetAddressOf(), m_DepthStencil->GetDSV().Get());
	}

	void GraphicDevice_Dx11::UpdateViewPort()
	{
		// viewport update
		HWND hWnd = application.GetHwnd();
		RECT winRect = {};
		GetClientRect(hWnd, &winRect);
		m_ViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)
			, (float)(winRect.bottom - winRect.top)
			, 0.0f, 1.0f
		};

		BindViewPort(&m_ViewPort);
	}

	void GraphicDevice_Dx11::Draw()
	{
		//renderer::m_Mesh->BindBuffer();
		//renderer::m_Shader->Binds();
		//m_Context->DrawIndexed(renderer::m_Mesh->GetIndexCount(), 0, 0);
	}

	void GraphicDevice_Dx11::Present()
	{
		m_SwapChain->Present(0, 0);
	}
}

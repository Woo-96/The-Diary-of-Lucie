#pragma once
#include "Lu_Resource.h"
#include "Lu_GraphicDevice_Dx11.h"

namespace Lu
{
	using namespace graphics;

	class Mesh : public Resource
	{
	public:
		Mesh();
		virtual ~Mesh();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_IndexBuffer;

		D3D11_BUFFER_DESC						m_VBDesc;
		D3D11_BUFFER_DESC						m_IBDesc;

		UINT									m_IndexCount;

	public:
		UINT GetIndexCount() 
		{ 
			return m_IndexCount; 
		}

	public:
		bool CreateVertexBuffer(void* _Data, UINT _Count);
		bool CreateIndexBuffer(void* _Data, UINT _Count);

		void BindBuffer();
		void Render();
		void RenderInstanced(UINT _StartIndexLocation);

	public:
		virtual HRESULT Load(const std::wstring& _Path) override;
	};
}
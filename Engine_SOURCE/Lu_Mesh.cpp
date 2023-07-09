#include "Lu_Mesh.h"
#include "Lu_Renderer.h"

namespace Lu
{
	Mesh::Mesh()
		: Resource(enums::eResourceType::Mesh)
		, m_VertexBuffer(nullptr)
		, m_IndexBuffer(nullptr)
		, m_VBDesc{}
		, m_IBDesc{}
		, m_IndexCount(0)
	{
	}

	Mesh::~Mesh()
	{
	}

	bool Mesh::CreateVertexBuffer(void* _Data, UINT _Count)
	{
		m_VBDesc.ByteWidth = sizeof(renderer::Vertex) * _Count;
		m_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		m_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		m_VBDesc.CPUAccessFlags = 0; // NONE

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = _Data;

		if (!GetDevice()->CreateBuffer(m_VertexBuffer.GetAddressOf(), &m_VBDesc, &sub))
			return false;

		return true;
	}

	bool Mesh::CreateIndexBuffer(void* _Data, UINT _Count)
	{
		m_IndexCount = _Count;
		m_IBDesc.ByteWidth = sizeof(UINT) * _Count;
		m_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		m_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		m_IBDesc.CPUAccessFlags = 0; // NONE

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = _Data;

		if (!GetDevice()->CreateBuffer(m_IndexBuffer.GetAddressOf(), &m_IBDesc, &sub))
			return false;

		return true;
	}

	void Mesh::BindBuffer()
	{
		UINT stride = sizeof(renderer::Vertex); // 버퍼 크기
		UINT offset = 0;						// 버퍼 시작 위치. 0으로 하면 됨

		GetDevice()->BindVertexBuffer(0, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		GetDevice()->BindIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::Render()
	{
		GetDevice()->DrawIndexed(m_IndexCount, 0, 0);
	}

	HRESULT Mesh::Load(const std::wstring& _Path)
	{
		return E_NOTIMPL;
	}
}

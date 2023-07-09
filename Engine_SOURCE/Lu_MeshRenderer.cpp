#include "Lu_MeshRenderer.h"
#include "Lu_GameObject.h"
#include "Lu_Transform.h"
#include "Lu_Renderer.h"

namespace Lu
{
	MeshRenderer::MeshRenderer()
		: Component(eComponentType::MeshRenderer)
	{
	}
	MeshRenderer::~MeshRenderer()
	{
	}
	void MeshRenderer::Initialize()
	{
	}
	void MeshRenderer::Update()
	{
	}
	void MeshRenderer::LateUpdate()
	{
	}
	void MeshRenderer::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->BindConstantBuffer();

		m_Mesh->BindBuffer();
		m_Material->Binds();
		m_Mesh->Render();

		m_Material->Clear();
	}
}
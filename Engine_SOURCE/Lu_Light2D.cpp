#include "Lu_Light2D.h"
#include "Lu_Renderer.h"
#include "Lu_Transform.h"
#include "Lu_GameObject.h"

namespace Lu
{
	Light2D::Light2D()
		: Component(eComponentType::Light)
	{
	}

	Light2D::~Light2D()
	{
	}
	
	void Light2D::Initialize()
	{
	}
	
	void Light2D::Update()
	{
	}
	
	void Light2D::LateUpdate()
	{
		renderer::lights.push_back(this);

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		m_Attribute.Position = Vector4(pos.x, pos.y, pos.z, 1.0f);
		m_Attribute.Direction = Vector4(tr->Foward().x, tr->Foward().y, tr->Foward().z, 1.0f);
	}
	
	void Light2D::Render()
	{
	}
}
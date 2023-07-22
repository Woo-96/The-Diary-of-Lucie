#include "Lu_Collider2D.h"
#include "Lu_GameObject.h"
#include "Lu_Renderer.h"

// 테스트용
#include "Lu_Object.h"
#include "Lu_Time.h"

namespace Lu
{
	UINT Collider2D::m_ColliderNumber = 0;

	Collider2D::Collider2D()
		: Component(eComponentType::Collider2D)
		, m_Type(eColliderType::Rect)
		, m_Size(Vector2::One)
		, m_Center(Vector2::Zero)
	{
		SetName(L"Collier2D");
		m_ColliderNumber++;
		m_ColliderID = m_ColliderNumber;
	}

	Collider2D::~Collider2D()
	{

	}

	void Collider2D::Initialize()
	{
		
	}

	void Collider2D::Update()
	{

	}

	void Collider2D::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 scale = tr->GetScale();
		scale.x *= m_Size.x;
		scale.y *= m_Size.y;

		Vector3 pos = tr->GetPosition();
		pos.x += m_Center.x;
		pos.y += m_Center.y;

		m_Position = pos;

		graphics::DebugMesh mesh = {};
		mesh.Position = pos;
		mesh.Scale = scale;
		mesh.Rotation = tr->GetRotation();
		mesh.Type = eColliderType::Circle;

		renderer::PushDebugMeshAttribute(mesh);
	}

	void Collider2D::Render()
	{

	}

	void Collider2D::OnCollisionEnter(Collider2D* _Other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionEnter(_Other);
		}
	}

	void Collider2D::OnCollisionStay(Collider2D* _Other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionStay(_Other);
		}
	}

	void Collider2D::OnCollisionExit(Collider2D* _Other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(_Other);
		}
	}
}

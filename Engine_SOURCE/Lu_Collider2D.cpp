#include "Lu_Collider2D.h"
#include "Lu_GameObject.h"
#include "Lu_Renderer.h"

// 테스트용
#include "Lu_Input.h"

namespace Lu
{
	UINT Collider2D::m_ColliderNumber = 0;

	Collider2D::Collider2D()
		: Component(eComponentType::Collider2D)
		, m_Type(eColliderType::Rect)
		, m_Size(Vector2::One)
		, m_Center(Vector2::Zero)
		, m_CollisionCount(0)
		, m_bRender(true)
	{
		SetName(L"Collier2D");
		m_ColliderNumber++;
		m_ColliderID = m_ColliderNumber;
	}

	Collider2D::~Collider2D()
	{

	}

	void Collider2D::Update()
	{
		if (Input::GetKeyDown(eKeyCode::K))
		{
			if (m_bRender)
				m_bRender = false;
			else
				m_bRender = true;
		}
	}

	void Collider2D::LateUpdate()
	{
		assert(0 <= m_CollisionCount);

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
		mesh.Type = m_Type;

		Vector4 vColor;
		if(0 < m_CollisionCount)
			vColor = Vector4(1.f, 0.f, 0.f, 1.f);
		else
			vColor = Vector4(0.f, 1.f, 0.f, 1.f);

		mesh.Color = vColor;

		if(m_bRender)
			renderer::PushDebugMeshAttribute(mesh);
	}

	void Collider2D::OnCollisionEnter(Collider2D* _Other)
	{
		++m_CollisionCount;

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
		--m_CollisionCount;

		const std::vector<Script*>& scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(_Other);
		}
	}
}

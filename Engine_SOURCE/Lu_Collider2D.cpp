#include "Lu_Collider2D.h"
#include "Lu_GameObject.h"
#include "Lu_Renderer.h"

// �׽�Ʈ��
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
		, m_FinalTransform(Matrix::Identity)
	{
		SetName(L"Collier2D");
		m_ColliderNumber++;
		m_ColliderID = m_ColliderNumber;
	}

	Collider2D::~Collider2D()
	{

	}

	void Collider2D::LateUpdate()
	{
		assert(0 <= m_CollisionCount);

		Transform* tr = GetOwner()->GetComponent<Transform>();

		// ũ�� ���� ����
		Vector3 scale = tr->GetScale();
		scale.x *= m_Size.x;
		scale.y *= m_Size.y;

		// ��ġ ������ ����
		Vector3 pos = tr->GetPosition();
		pos.x += m_Center.x;
		pos.y += m_Center.y;

		m_Position = pos;

		// ���� ���
		Vector3 rotation = tr->GetRotation();
		Matrix scaleMatrix = Matrix::CreateScale(scale.x, scale.y, 1.0f); // ������ ���
		Matrix rotationMatrix = Matrix::CreateRotationX(rotation.x) * // X �� ȸ��
			Matrix::CreateRotationY(rotation.y) * // Y �� ȸ��
			Matrix::CreateRotationZ(rotation.z);  // Z �� ȸ��
		Matrix translationMatrix = Matrix::CreateTranslation(pos.x, pos.y, pos.z); // �̵� ���
		m_FinalTransform = scaleMatrix * rotationMatrix * translationMatrix; // ���� ��ȯ ���


		// ����� ������ ��û
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

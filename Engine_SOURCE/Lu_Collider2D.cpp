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

		// 크기 배율 적용
		Vector3 scale = tr->GetScale();
		scale.x *= m_Size.x;
		scale.y *= m_Size.y;

		// 위치 오프셋 적용
		Vector3 pos = tr->GetPosition();
		pos.x += m_Center.x;
		pos.y += m_Center.y;

		m_Position = pos;

		// 최종 행렬
		Vector3 rotation = tr->GetRotation();
		Matrix scaleMatrix = Matrix::CreateScale(scale.x, scale.y, 1.0f); // 스케일 행렬
		Matrix rotationMatrix = Matrix::CreateRotationX(rotation.x) * // X 축 회전
			Matrix::CreateRotationY(rotation.y) * // Y 축 회전
			Matrix::CreateRotationZ(rotation.z);  // Z 축 회전
		Matrix translationMatrix = Matrix::CreateTranslation(pos.x, pos.y, pos.z); // 이동 행렬
		m_FinalTransform = scaleMatrix * rotationMatrix * translationMatrix; // 최종 변환 행렬


		// 디버그 렌더링 요청
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

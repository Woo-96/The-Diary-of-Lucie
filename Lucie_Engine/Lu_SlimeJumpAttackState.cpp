#include "Lu_SlimeJumpAttackState.h"
#include "Lu_SlimeScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_CircleProjectile.h"
#include "Lu_Resources.h"

namespace Lu
{
	SlimeJumpAttackState::SlimeJumpAttackState()
	{
		SetName(L"SlimeJumpAttackStateScript");
		SetStateType(eState::JumpAttack);
	}

	SlimeJumpAttackState::~SlimeJumpAttackState()
	{

	}

	void SlimeJumpAttackState::Update()
	{

	}

	void SlimeJumpAttackState::Enter()
	{

	}

	void SlimeJumpAttackState::Exit()
	{
		//Vector3 vMonsterPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		//vMonsterPos.y -= 50.f;
		//vMonsterPos.z = 600.f;

		//const int numProjectiles = 8;
		//const float angleIncrement = 360.0f / numProjectiles; // 8�������� �������� ���� ���� ���

		//// ��Ŭ ����ü ����
		//for (int i = 0; i < numProjectiles; ++i)
		//{
		//	GameObject* pProjectile = object::Instantiate<GameObject>(vMonsterPos, Vector3(80.f, 80.f, 100.f), eLayerType::MonsterProjectile);
		//	pProjectile->SetName(L"CircleProjectile");

		//	MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		//	pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		//	pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_BossCircle_Mtrl"));

		//	Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
		//	pCollider->SetType(eColliderType::Rect);
		//	pCollider->SetSize(Vector2(0.6f, 0.6f));

		//	CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
		//	pProjectileScript->SetMonsterScript((MonsterScript*)this);
		//	pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
		//	pProjectileScript->SetSpeed(300.f);

		//	float angle = i * angleIncrement; // ����ü�� ������ �����ϴ� ���� ���
		//	float angleInRadians = DegreeToRadian(angle); // ������ �������� ��ȯ
		//	// �ﰢ�Լ��� �̿��Ͽ� ���� ���� ���
		//	float cosAngle = cos(angleInRadians);
		//	float sinAngle = sin(angleInRadians);
		//	Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // ����ü�� ������ ���� ���� ���
		//	pProjectileScript->SetDir(forwardDirection);
		//}
	}
}
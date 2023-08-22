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
		//const float angleIncrement = 360.0f / numProjectiles; // 8방향으로 퍼지도록 각도 간격 계산

		//// 써클 투사체 생성
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

		//	float angle = i * angleIncrement; // 투사체의 방향을 결정하는 각도 계산
		//	float angleInRadians = DegreeToRadian(angle); // 각도를 라디안으로 변환
		//	// 삼각함수를 이용하여 방향 벡터 계산
		//	float cosAngle = cos(angleInRadians);
		//	float sinAngle = sin(angleInRadians);
		//	Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // 투사체가 전진할 방향 벡터 계산
		//	pProjectileScript->SetDir(forwardDirection);
		//}
	}
}
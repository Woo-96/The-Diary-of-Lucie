#include "Lu_SlimeAttackState.h"
#include "Lu_SlimeScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_CircleProjectile.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	SlimeAttackState::SlimeAttackState()
	{
		SetName(L"SlimeAttackStateScript");
		SetStateType(eState::Attack);
	}

	SlimeAttackState::~SlimeAttackState()
	{

	}

	void SlimeAttackState::Update()
	{

	}

	void SlimeAttackState::Enter()
	{
		// 방향 결정
		Vector3 vDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vDir.Normalize();
		SetDir(vDir);

		// 애니메이션 방향 결정
		DetermineAnimDir(vDir);
		m_AttackDir = vDir;
	}

	void SlimeAttackState::Exit()
	{
		//// 투사체 발사
		//Vector3 baseDir = m_AttackDir;
		//baseDir.Normalize();

		//// 각도 변환을 위한 라디안 계산
		//float angle15 = math::DegreeToRadian(15.0f);

		//// 투사체 생성과 설정
		//for (int i = 0; i < 3; ++i)
		//{
		//	GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(50.f, 50.f, 100.f), eLayerType::MonsterProjectile);
		//	pProjectile->SetName(L"SlimeProjectile");

		//	MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		//	pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		//	pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_Circle_Mtrl"));

		//	Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
		//	pCollider->SetType(eColliderType::Rect);
		//	pCollider->SetSize(Vector2(0.6f, 0.6f));

		//	CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
		//	pProjectileScript->SetMonsterScript((MonsterScript*)GetSlimeScript());
		//	pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());

		//	// 첫 번째 투사체는 기본 방향으로 설정
		//	if (i == 0)
		//	{
		//		pProjectileScript->SetDir(baseDir);
		//	}
		//	else
		//	{
		//		// 나머지 투사체는 각도 조절하여 설정
		//		Vector3 dir = baseDir;
		//		if (i == 1)
		//		{
		//			// -15도 각도 조절
		//			dir = Vector3(std::cos(angle15) * baseDir.x - std::sin(angle15) * baseDir.y,
		//				std::sin(angle15) * baseDir.x + std::cos(angle15) * baseDir.y,
		//				0.0f);
		//		}
		//		else if (i == 2)
		//		{
		//			// +15도 각도 조절
		//			dir = Vector3(std::cos(-angle15) * baseDir.x - std::sin(-angle15) * baseDir.y,
		//				std::sin(-angle15) * baseDir.x + std::cos(-angle15) * baseDir.y,
		//				0.0f);
		//		}

		//		dir.Normalize();
		//		pProjectileScript->SetDir(dir);
		//	}
		//}
	}
}
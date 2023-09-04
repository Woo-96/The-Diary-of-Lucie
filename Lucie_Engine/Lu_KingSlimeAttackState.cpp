#include "Lu_KingSlimeAttackState.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_BubbleProjectile.h"

namespace Lu
{
	KingSlimeAttackState::KingSlimeAttackState()
	{
		SetName(L"KingSlimeAttackStateScript");
		SetStateType(eState::Attack);
	}

	KingSlimeAttackState::~KingSlimeAttackState()
	{

	}

	void KingSlimeAttackState::Enter()
	{

	}

	void KingSlimeAttackState::Exit()
	{
		Vector3 vBossPos = GetTransform()->GetPosition();
		vBossPos.y -= 150.f;
		vBossPos.z = 550.f;

		const int numProjectiles = 8;
		const float angleIncrement = 360.0f / numProjectiles; // 8방향으로 퍼지도록 각도 간격 계산

		for (int i = 0; i < numProjectiles; ++i)
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(vBossPos, Vector3(200.f, 200.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"BubbleProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_Bubble_Mtrl"));


			BubbleProjectile* pProjectileScript = pProjectile->AddComponent<BubbleProjectile>();
			pProjectileScript->SetMonsterScript((MonsterScript*)this);
			pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
			pProjectileScript->SetSpeed(250.f);

			float angle = i * angleIncrement; // 투사체의 방향을 결정하는 각도 계산
			float angleInRadians = DegreeToRadian(angle); // 각도를 라디안으로 변환
			// 삼각함수를 이용하여 방향 벡터 계산
			float cosAngle = cos(angleInRadians);
			float sinAngle = sin(angleInRadians);
			Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // 투사체가 전진할 방향 벡터 계산
			pProjectileScript->SetDir(forwardDirection);
		}
	}
}
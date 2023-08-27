#include "Lu_SlimeJumpAttackState.h"
#include "Lu_SlimeScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_CircleProjectile.h"
#include "Lu_Resources.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	SlimeJumpAttackState::SlimeJumpAttackState()
	{
		SetName(L"SlimeJumpAttackStateScript");
		SetStateType(eState::JumpAttack);

		m_SFX = new GameObject;
		m_SFX->AddComponent<AudioSource>();
	}

	SlimeJumpAttackState::~SlimeJumpAttackState()
	{
		if (nullptr != m_SFX)
		{
			delete m_SFX;
			m_SFX = nullptr;
		}
	}

	void SlimeJumpAttackState::Enter()
	{
		AudioSource* pAudio = m_SFX->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"SlimeJumpSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeJumpSFX.ogg"));
		pAudio->Play();
	}

	void SlimeJumpAttackState::Exit()
	{
		Vector3 vMonsterPos = GetSlimeScript()->GetOwner()->GetComponent<Transform>()->GetPosition();
		vMonsterPos.y -= 35.f;
		vMonsterPos.z = 600.f;

		const int numProjectiles = 8;
		const float angleIncrement = 360.0f / numProjectiles; // 8방향으로 퍼지도록 각도 간격 계산

		// 써클 투사체 생성
		for (int i = 0; i < numProjectiles; ++i)
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(vMonsterPos, Vector3(50.f, 50.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"SlimeCircleProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_Circle_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.6f, 0.6f));

			CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
			pProjectileScript->SetMonsterScript((MonsterScript*)this);
			pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
			pProjectileScript->SetSpeed(200.f);

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
#include "Lu_BigSlimeAttackState.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_BubbleProjectile.h"
#include "Lu_JumpProjectile.h"
#include "Lu_BigSlimeScript.h"
#include "Lu_AudioSource.h"
#include "Lu_CircleProjectile.h"

namespace Lu
{
	BigSlimeAttackState::BigSlimeAttackState()
	{
		SetName(L"BigSlimeAttackStateScript");
		SetStateType(eState::Attack);

		m_SFX = new GameObject;
		m_SFX->AddComponent<AudioSource>();
	}

	BigSlimeAttackState::~BigSlimeAttackState()
	{
		if (nullptr != m_SFX)
		{
			delete m_SFX;
			m_SFX = nullptr;
		}
	}

	void BigSlimeAttackState::Enter()
	{
		Vector3 vSpawnPos = GetBigSlimeScript()->GetOwner()->GetComponent<Transform>()->GetPosition();
		vSpawnPos.y -= 100.f;
		vSpawnPos.z = 600.f;

		GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(1000.f, 1000.f, 100.f), eLayerType::MonsterProjectile);
		pProjectile->SetName(L"JumpProjectile");

		MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeJumpAnimation_Mtrl"));

		Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
		pCollider->SetType(eColliderType::Rect);
		pCollider->SetSize(Vector2(0.5f, 0.3f));

		Animator* pAnimator = pProjectile->AddComponent<Animator>();
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"BigSlimeJump_TEX", L"..\\Resources\\Texture\\Monster\\MidBoss\\BigSlimeJump.png");

		pAnimator->Create(L"BigSlime_JumpProjectile", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 7, Vector2(192.f, 192.f), Vector2::Zero, 0.1f);
		pAnimator->PlayAnimation(L"BigSlime_JumpProjectile", false);

		pProjectile->AddComponent<JumpProjectile>();

		// SFX
		AudioSource* pAudio = m_SFX->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"SlimeJumpSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeJumpSFX.ogg"));
		pAudio->Play();
	}

	void BigSlimeAttackState::Exit()
	{
		Vector3 vBossPos = GetBigSlimeScript()->GetOwner()->GetComponent<Transform>()->GetPosition();
		vBossPos.y -= 150.f;
		vBossPos.z = 600.f;

		const int numProjectiles = 8;
		const float angleIncrement = 360.0f / numProjectiles; // 8방향으로 퍼지도록 각도 간격 계산

		// 써클 투사체 생성
		for (int i = 0; i < numProjectiles; ++i)
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(vBossPos, Vector3(80.f, 80.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"CircleProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_SlimeCircle_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.6f, 0.6f));

			CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
			pProjectileScript->SetMonsterScript((MonsterScript*)this);
			pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
			pProjectileScript->SetSpeed(300.f);

			float angle = i * angleIncrement; // 투사체의 방향을 결정하는 각도 계산
			float angleInRadians = DegreeToRadian(angle); // 각도를 라디안으로 변환
			// 삼각함수를 이용하여 방향 벡터 계산
			float cosAngle = cos(angleInRadians);
			float sinAngle = sin(angleInRadians);
			Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // 투사체가 전진할 방향 벡터 계산
			pProjectileScript->SetDir(forwardDirection);
		}


		AudioSource* pAudio = m_SFX->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"SlimeBubbleSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeBubbleSFX.ogg"));
		pAudio->Play();
	}
}
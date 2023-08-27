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
		const float angleIncrement = 360.0f / numProjectiles; // 8�������� �������� ���� ���� ���

		// ��Ŭ ����ü ����
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

			float angle = i * angleIncrement; // ����ü�� ������ �����ϴ� ���� ���
			float angleInRadians = DegreeToRadian(angle); // ������ �������� ��ȯ
			// �ﰢ�Լ��� �̿��Ͽ� ���� ���� ���
			float cosAngle = cos(angleInRadians);
			float sinAngle = sin(angleInRadians);
			Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // ����ü�� ������ ���� ���� ���
			pProjectileScript->SetDir(forwardDirection);
		}


		AudioSource* pAudio = m_SFX->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"SlimeBubbleSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeBubbleSFX.ogg"));
		pAudio->Play();
	}
}
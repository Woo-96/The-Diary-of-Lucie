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
		const float angleIncrement = 360.0f / numProjectiles; // 8�������� �������� ���� ���� ���

		// ��Ŭ ����ü ����
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

			float angle = i * angleIncrement; // ����ü�� ������ �����ϴ� ���� ���
			float angleInRadians = DegreeToRadian(angle); // ������ �������� ��ȯ
			// �ﰢ�Լ��� �̿��Ͽ� ���� ���� ���
			float cosAngle = cos(angleInRadians);
			float sinAngle = sin(angleInRadians);
			Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // ����ü�� ������ ���� ���� ���
			pProjectileScript->SetDir(forwardDirection);
		}
	}
}
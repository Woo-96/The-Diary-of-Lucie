#include "Lu_TomatoBoomProjectile.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_DamageFontScript.h"
#include "Lu_ProjectileScript.h"
#include "Lu_Time.h"
#include "Lu_CircleProjectile.h"

namespace Lu
{
	TomatoBoomProjectile::TomatoBoomProjectile()
		: m_HP(30)
		, m_HitCoolTime(0.f)
	{
		SetName(L"TomatoBoomProjectileScript");
	}

	TomatoBoomProjectile::~TomatoBoomProjectile()
	{
	}

	void TomatoBoomProjectile::Initialize()
	{
		MonsterProjectileScript::Initialize();
	}

	void TomatoBoomProjectile::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 1.f && GetTime() < 1.5f)
		{
			GetAnimator()->PlayAnimation(L"TomatoBoom_Blink", true);
		}

		else if (GetTime() >= 1.5f)
		{
			GetAnimator()->PlayAnimation(L"TomatoBoom_Boom", true);
		}
	}

	void TomatoBoomProjectile::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			ProjectileScript* pProjectile = _Other->GetOwner()->GetComponent<ProjectileScript>();
			InflictDamage(pProjectile->GetProjectileDamage());

			if (m_HP <= 0.f)
			{
				object::Destroy(GetOwner());
			}
		}
	}

	void TomatoBoomProjectile::OnCollisionStay(Collider2D* _Other)
	{
		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			m_HitCoolTime += (float)Time::DeltaTime();

			if (m_HitCoolTime >= 0.1f)
			{
				ProjectileScript* pProjectile = _Other->GetOwner()->GetComponent<ProjectileScript>();
				InflictDamage(pProjectile->GetProjectileDamage());

				if (m_HP <= 0.f)
				{
					object::Destroy(GetOwner());
				}

				m_HitCoolTime = 0;
			}
		}
	}

	void TomatoBoomProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"TomatoBoom_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\TomatoBoom.png");

		GetAnimator()->Create(L"TomatoBoom_Idle", pAtlas, Vector2(0.f, 0.f), Vector2(27.f, 27.f), 1, Vector2(27.f, 27.f));
		GetAnimator()->Create(L"TomatoBoom_Blink", pAtlas, Vector2(27.f, 0.f), Vector2(27.f, 27.f), 2, Vector2(27.f, 27.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"TomatoBoom_Boom", pAtlas, Vector2(27.f, 0.f), Vector2(27.f, 27.f), 3, Vector2(27.f, 27.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"TomatoBoom_Boom") = std::bind(&TomatoBoomProjectile::Boom, this);

		GetAnimator()->PlayAnimation(L"TomatoBoom_Idle", true);
	}

	void TomatoBoomProjectile::InflictDamage(int _Damage)
	{
		// ���� ����
		m_HP -= _Damage;

		// ����� ��Ʈ ����
		Vector3 vPos = GetOwner()->GetComponent<Collider2D>()->GetPosition();
		vPos.y += GetOwner()->GetComponent<Transform>()->GetScale().y / 2.f;
		vPos.z = 200.f;
		GameObject* pObject = object::Instantiate<GameObject>(vPos, Vector3(20.f, 20.f, 100.f), eLayerType::DamageFont);
		pObject->SetName(L"DamageFont");
		MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"DamageFont_Mtrl"));
		DamageFontScript* pDamageFont = pObject->AddComponent<DamageFontScript>();
		pDamageFont->SetDamage(_Damage);
	}

	void TomatoBoomProjectile::Boom()
	{
		const int numProjectiles = 8;
		const float angleIncrement = 360.0f / numProjectiles; // 8�������� �������� ���� ���� ���

		for (int i = 0; i < numProjectiles; ++i)
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(50.f, 50.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"CircleProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_Circle_Mtrl"));

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


		object::Destroy(GetOwner());
	}
}
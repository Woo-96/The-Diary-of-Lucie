#include "Lu_TomatoBombProjectile.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_DamageFontScript.h"
#include "Lu_ProjectileScript.h"
#include "Lu_Time.h"
#include "Lu_CircleProjectile.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	TomatoBombProjectile::TomatoBombProjectile()
		: m_HP(30)
		, m_HitCoolTime(0.f)
		, m_Random(Vector2::Zero)
	{
		SetName(L"TomatoBombProjectileScript");
	}

	TomatoBombProjectile::~TomatoBombProjectile()
	{
	}

	void TomatoBombProjectile::Initialize()
	{
		MonsterProjectileScript::Initialize();

		m_Random.x = (float)(std::rand() % 601 - 300);
		m_Random.y = (float)(std::rand() % (500 - 350 + 1) + 350);
	}

	void TomatoBombProjectile::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() <= 0.5f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			vPos.y += 100.f * (float)Time::DeltaTime();
			GetTransform()->SetPosition(vPos);
		}
		else if (0.5f < GetTime() && GetTime() <= 2.f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			vPos.x -= m_Random.x * (float)Time::DeltaTime();
			vPos.y -= m_Random.y * (float)Time::DeltaTime();
			GetTransform()->SetPosition(vPos);
		}
		else if (GetTime() > 5.f && GetTime() < 6.f)
		{
			GetAnimator()->PlayAnimation(L"TomatoBomb_Blink", true);
		}
		else if (GetTime() >= 6.f)
		{
			GetAnimator()->PlayAnimation(L"TomatoBomb_Bomb", true);
		}
	}

	void TomatoBombProjectile::OnCollisionEnter(Collider2D* _Other)
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

	void TomatoBombProjectile::OnCollisionStay(Collider2D* _Other)
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

	void TomatoBombProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"TomatoBomb_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\TomatoBomb.png");

		GetAnimator()->Create(L"TomatoBomb_Idle", pAtlas, Vector2(0.f, 0.f), Vector2(27.f, 27.f), 1, Vector2(27.f, 27.f));
		GetAnimator()->Create(L"TomatoBomb_Blink", pAtlas, Vector2(27.f, 0.f), Vector2(27.f, 27.f), 2, Vector2(27.f, 27.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"TomatoBomb_Bomb", pAtlas, Vector2(27.f, 0.f), Vector2(27.f, 27.f), 3, Vector2(27.f, 27.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"TomatoBomb_Bomb") = std::bind(&TomatoBombProjectile::Bomb, this);

		GetAnimator()->PlayAnimation(L"TomatoBomb_Idle", true);
	}

	void TomatoBombProjectile::InflictDamage(int _Damage)
	{
		// 피해 적용
		m_HP -= _Damage;

		// 대미지 폰트 생성
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

	void TomatoBombProjectile::Bomb()
	{
		const int numProjectiles = 8;
		const float angleIncrement = 360.0f / numProjectiles; // 8방향으로 퍼지도록 각도 간격 계산

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

			float angle = i * angleIncrement; // 투사체의 방향을 결정하는 각도 계산
			float angleInRadians = DegreeToRadian(angle); // 각도를 라디안으로 변환
			// 삼각함수를 이용하여 방향 벡터 계산
			float cosAngle = cos(angleInRadians);
			float sinAngle = sin(angleInRadians);
			Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // 투사체가 전진할 방향 벡터 계산
			pProjectileScript->SetDir(forwardDirection);
		}

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntTomatoBombSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntTomatoBombSFX.ogg"));
		pSFX->Play();


		object::Destroy(GetOwner());
	}
}
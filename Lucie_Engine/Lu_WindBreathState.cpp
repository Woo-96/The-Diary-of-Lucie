#include "Lu_WindBreathState.h"
#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"
#include "Lu_WindBreathProjectile.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

namespace Lu
{
	WindBreathState::WindBreathState()
		: m_BreathDir(0.f, -1.f, 0.f)
	{
		SetName(L"WindBreathStateScript");
		SetStateType(eState::Attack_WindBreath);
	}

	WindBreathState::~WindBreathState()
	{
	}

	void WindBreathState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::WindBreath);
	}

	void WindBreathState::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() <= 6.f)
		{
			if(GetTime() >= 1.f && GetTime() < 1.2f)
				m_BreathDir.x += 0.3 * (float)Time::DeltaTime();
			else if(GetTime() >= 1.2f && GetTime() < 1.6f)
				m_BreathDir.x -= 0.3 * (float)Time::DeltaTime();
			else if(GetTime() >= 1.6f && GetTime() < 2.2f)
				m_BreathDir.x += 0.3 * (float)Time::DeltaTime();
			else if (GetTime() >= 2.2f && GetTime() < 3.f)
				m_BreathDir.x -= 0.3 * (float)Time::DeltaTime();
			else if (GetTime() >= 3.f && GetTime() < 4.f)
				m_BreathDir.x += 0.3 * (float)Time::DeltaTime();
			else if (GetTime() >= 4.f && GetTime() < 4.6f)
				m_BreathDir.x -= 0.3 * (float)Time::DeltaTime();

			SetAttackCoolTime(GetAttackCoolTime() + (float)Time::DeltaTime());

			if (GetAttackCoolTime() >= 0.2f)
			{
				CreateProjectile();
				SetAttackCoolTime(0.f);
			}
		}
		else
		{
			GetEntScript()->ChangeState(eState::Idle);
		}
	}

	void WindBreathState::Enter()
	{
		EntAttackState::Enter();
	}

	void WindBreathState::Exit()
	{
		EntAttackState::Exit();

		m_BreathDir = Vector3(0.f, -1.f, 0.f);
	}

	void WindBreathState::ChangeAnimation()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_WindBreath_Phase1", false);
			break;
		case EntScript::ePhase::Phase_2:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_WindBreath_Phase2", false);
			break;
		}
	}

	void WindBreathState::CreateProjectile()
	{
		// 각도 변환을 위한 라디안 계산
		float angle15 = math::DegreeToRadian(15.0f);

		Vector3 vSpawnPos = GetTransform()->GetPosition();
		vSpawnPos.y -= 250.f;

		// 투사체 생성과 설정
		for (int i = 0; i < 3; ++i)
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(90.f, 90.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"WindBreath");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Ent_Attack_WindBreath_Mtrl"));

			pProjectile->AddComponent<Collider2D>()->SetSize(Vector2(0.4f, 0.4f));
			pProjectile->AddComponent<Animator>();

			WindBreathProjectile* pProjectileScript = pProjectile->AddComponent<WindBreathProjectile>();
			pProjectileScript->SetMonsterScript((MonsterScript*)GetEntScript());
			pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());

			// 첫 번째 투사체는 기본 방향으로 설정
			if (i == 0)
			{
				pProjectileScript->SetDir(m_BreathDir);
			}
			else
			{
				// 나머지 투사체는 각도 조절하여 설정
				Vector3 dir = m_BreathDir;
				if (i == 1)
				{
					// -15도 각도 조절
					dir = Vector3(std::cos(angle15) * m_BreathDir.x - std::sin(angle15) * m_BreathDir.y,
						std::sin(angle15) * m_BreathDir.x + std::cos(angle15) * m_BreathDir.y,
						0.0f);
				}
				else if (i == 2)
				{
					// +15도 각도 조절
					dir = Vector3(std::cos(-angle15) * m_BreathDir.x - std::sin(-angle15) * m_BreathDir.y,
						std::sin(-angle15) * m_BreathDir.x + std::cos(-angle15) * m_BreathDir.y,
						0.0f);
				}

				dir.Normalize();
				pProjectileScript->SetDir(dir);
			}
		}
	}
}
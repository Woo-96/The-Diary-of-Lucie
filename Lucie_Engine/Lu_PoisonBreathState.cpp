#include "Lu_PoisonBreathState.h"
#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_PoisonBreathProjectile.h"
#include "Lu_TraceCircleProjectile.h"

namespace Lu
{
	PoisonBreathState::PoisonBreathState()
		: m_StartDir(ePoisonDir::Left)
	{
		SetName(L"PoisonBreathStateScript");
		SetStateType(eState::Attack_PoisonBreath);
	}

	PoisonBreathState::~PoisonBreathState()
	{
	}

	void PoisonBreathState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::PoisonBreath);
	}

	void PoisonBreathState::Update()
	{
		float DT = (float)Time::DeltaTime();
		SetTime(GetTime() + DT);

		if (GetTime() >= 0.5f)
		{
			switch (GetEntScript()->GetPhase())
			{
			case EntScript::ePhase::Phase_1:
			{
				switch (m_StartDir)
				{
				case Lu::PoisonBreathState::ePoisonDir::Left:
					GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_PoisonBreath_Phase1_Left", true);
					break;
				case Lu::PoisonBreathState::ePoisonDir::Right:
					GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_PoisonBreath_Phase1_Right", true);
					break;
				default:
					break;
				}
			}
				break;
			case EntScript::ePhase::Phase_2:
			{
				switch (m_StartDir)
				{
				case Lu::PoisonBreathState::ePoisonDir::Left:
					GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_PoisonBreath_Phase2_Left", true);
					break;
				case Lu::PoisonBreathState::ePoisonDir::Right:
					GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_PoisonBreath_Phase2_Right", true);
					break;
				default:
					break;
				}
			}
				break;
			}



			SetAttackCoolTime(GetAttackCoolTime() + DT);

			if (GetAttackCoolTime() >= 0.2f)
			{
				CreateProjectile();
				SetAttackCoolTime(0.f);
			}


			// 두 번째 투사체
			if (GetTime() >= 2.5f)
			{
				m_CoolTime += DT;

				if (m_CoolTime >= 0.2f)
				{
					float Rand = (float)(std::rand() % 51 - 50);
					Vector3 vSpawnPos = GetTransform()->GetPosition();
					vSpawnPos.x += Rand;
					vSpawnPos.y -= 250.f;

					GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(50.f, 50.f, 100.f), eLayerType::MonsterProjectile);
					pProjectile->SetName(L"TraceProjectile");

					MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
					pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
					pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_Circle_Mtrl"));

					pProjectile->AddComponent<Collider2D>()->SetSize(Vector2(0.6f, 0.6f));

					TraceCircleProjectile* pProjectileScript = pProjectile->AddComponent<TraceCircleProjectile>();
					pProjectileScript->SetMonsterScript((MonsterScript*)this);
					pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
					pProjectileScript->SetSpeed((float)(std::rand() % 301 + 200));
					pProjectileScript->SetDir(Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f));

					m_CoolTime = 0.f;
				}
			}
		}
	}

	void PoisonBreathState::Enter()
	{
		EntAttackState::Enter();

		int iRand = std::rand() % 2;
		if (iRand == 0)
			m_StartDir = ePoisonDir::Left;
		else if (iRand == 1)
			m_StartDir = ePoisonDir::Right;
	}

	void PoisonBreathState::Exit()
	{
		EntAttackState::Exit();
	}

	void PoisonBreathState::ChangeAnimation()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_Change_Phase1", false);
			break;
		case EntScript::ePhase::Phase_2:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_Change_Phase2", false);
			break;
		}
	}

	void PoisonBreathState::CreateProjectile()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
		{
			Vector3 vSpawnPos = GetTransform()->GetPosition();
			vSpawnPos.y -= 250.f;

			GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(240.f, 240.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"PoisonBreath");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Ent_Attack_PoisonBreath_Mtrl"));

			pProjectile->AddComponent<Collider2D>()->SetSize(Vector2(0.5f, 0.5f));
			pProjectile->AddComponent<Animator>();

			PoisonBreathProjectile* pProjectileScript = pProjectile->AddComponent<PoisonBreathProjectile>();
			pProjectileScript->SetMonsterScript((MonsterScript*)GetEntScript());
			pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());

			if (m_StartDir == PoisonBreathState::ePoisonDir::Left)
			{
				pProjectileScript->SetDegree((GetTime() * 20));
			}
			else
			{
				pProjectileScript->SetDegree(180.f - (GetTime() * 20));
			}
		}
			break;
		case EntScript::ePhase::Phase_2:
		{
			Vector3 vSpawnPos = GetTransform()->GetPosition();
			vSpawnPos.y -= 250.f;

			for (int i = 0; i < 2; ++i)
			{
				GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(240.f, 240.f, 100.f), eLayerType::MonsterProjectile);
				pProjectile->SetName(L"PoisonBreath");

				MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"Ent_Attack_PoisonBreath_Mtrl"));

				pProjectile->AddComponent<Collider2D>()->SetSize(Vector2(0.5f, 0.5f));
				pProjectile->AddComponent<Animator>();

				PoisonBreathProjectile* pProjectileScript = pProjectile->AddComponent<PoisonBreathProjectile>();
				pProjectileScript->SetMonsterScript((MonsterScript*)GetEntScript());
				pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());

				if (0 == i)
				{
					pProjectileScript->SetDegree((GetTime() * 20));
				}
				else
				{
					pProjectileScript->SetDegree(180.f - (GetTime() * 20));
				}
			}
		}
			break;
		}
	}
}
#include "Lu_TomatoBombState.h"
#include "Lu_EntScript.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_TomatoBombProjectile.h"
#include "Lu_Resources.h"

namespace Lu
{
	TomatoBombState::TomatoBombState()
	{
		SetName(L"TomatoBombStateScript");
		SetStateType(eState::Attack_TomatoBomb);
	}

	TomatoBombState::~TomatoBombState()
	{
	}

	void TomatoBombState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::TomatoBomb);
	}

	void TomatoBombState::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 6.f)
		{
			switch (GetEntScript()->GetPhase())
			{
			case EntScript::ePhase::Phase_1:
				GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBomb_Phase1_End", true);
				break;
			case EntScript::ePhase::Phase_2:
				GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBomb_Phase2_End", true);
				break;
			}			
		}
		else if (GetTime() >= 0.5f && GetTime() <= 4.f)
		{
			SetAttackCoolTime(GetAttackCoolTime() + (float)Time::DeltaTime());

			if (GetAttackCoolTime() >= 1.f)
			{
				CreateProjectile();
				SetAttackCoolTime(0.f);
			}
		}
	}

	void TomatoBombState::Enter()
	{
		EntAttackState::Enter();
	}

	void TomatoBombState::Exit()
	{
		EntAttackState::Exit();
	}

	void TomatoBombState::ChangeAnimation()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBomb_Phase1_Start", false);
			break;
		case EntScript::ePhase::Phase_2:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBomb_Phase2_Start", false);
			break;
		}
	}

	void TomatoBombState::CreateProjectile()
	{
		Vector3 vSpawnPos = GetTransform()->GetPosition();
		vSpawnPos.y -= 200.f;

		GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(50.f, 50.f, 100.f), eLayerType::Monster);
		pProjectile->SetName(L"Tomato");

		MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Ent_Attack_TomatoBomb_Mtrl"));

		pProjectile->AddComponent<Collider2D>()->SetSize(Vector2(0.7f, 0.7f));
		pProjectile->AddComponent<Animator>();

		TomatoBombProjectile* pProjectileScript = pProjectile->AddComponent<TomatoBombProjectile>();
		pProjectileScript->SetMonsterScript((MonsterScript*)this);
		pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
	}
}
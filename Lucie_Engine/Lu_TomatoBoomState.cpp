#include "Lu_TomatoBoomState.h"
#include "Lu_EntScript.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	TomatoBoomState::TomatoBoomState()
	{
		SetName(L"TomatoBoomStateScript");
		SetStateType(eState::Attack_TomatoBoom);
	}

	TomatoBoomState::~TomatoBoomState()
	{
	}

	void TomatoBoomState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::TomatoBoom);

		GetEntScript()->GetAnimator()->CompleteEvent(L"Ent_Attack_TomatoBoom_Phase1_End") = std::bind(&TomatoBoomState::AttackFinished, this);
		GetEntScript()->GetAnimator()->CompleteEvent(L"Ent_Attack_TomatoBoom_Phase2_End") = std::bind(&TomatoBoomState::AttackFinished, this);
	}

	void TomatoBoomState::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 5.f)
		{
			switch (GetEntScript()->GetPhase())
			{
			case EntScript::ePhase::Phase_1:
				GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBoom_Phase1_End", true);
				break;
			case EntScript::ePhase::Phase_2:
				GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBoom_Phase2_End", true);
				break;
			}			
		}
		else if (GetTime() >= 0.5f)
		{
			SetAttackCoolTime(GetAttackCoolTime() + (float)Time::DeltaTime());

			if (GetAttackCoolTime() >= 0.5f)
			{
				CreateProjectile();
				SetAttackCoolTime(0.f);
			}
		}
	}

	void TomatoBoomState::Enter()
	{
		EntAttackState::Enter();
	}

	void TomatoBoomState::Exit()
	{
		EntAttackState::Exit();
	}

	void TomatoBoomState::ChangeAnimation()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBoom_Phase1_Start", false);
			break;
		case EntScript::ePhase::Phase_2:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_TomatoBoom_Phase2_Start", false);
			break;
		}
	}

	void TomatoBoomState::CreateProjectile()
	{
	}
}
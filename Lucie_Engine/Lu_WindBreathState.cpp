#include "Lu_WindBreathState.h"
#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"

namespace Lu
{
	WindBreathState::WindBreathState()
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

		if (GetTime() <= 3.f)
		{
			SetAttackCoolTime(GetAttackCoolTime() + (float)Time::DeltaTime());

			if (GetAttackCoolTime() >= 0.5f)
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
	}
}
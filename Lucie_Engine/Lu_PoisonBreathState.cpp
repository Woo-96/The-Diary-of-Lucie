#include "Lu_PoisonBreathState.h"
#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"

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
		SetTime(GetTime() + (float)Time::DeltaTime());

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
	}
}
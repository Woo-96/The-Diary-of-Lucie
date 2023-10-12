#include "Lu_CraterState.h"
#include "Lu_EntScript.h"
#include "Lu_Animator.h"

namespace Lu
{
	CraterState::CraterState()
	{
		SetName(L"CraterStateScript");
		SetStateType(eState::Attack_Crater);
	}

	CraterState::~CraterState()
	{
	}

	void CraterState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::Crater);
	}

	void CraterState::Update()
	{
	}

	void CraterState::Enter()
	{
		EntAttackState::Enter();
	}

	void CraterState::Exit()
	{
		EntAttackState::Exit();
	}

	void CraterState::ChangeAnimation()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Howling", true);
			break;
		case EntScript::ePhase::Phase_2:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Howling_Phase2", true);
			break;
		}
	}

	void CraterState::CreateProjectile()
	{
	}
}
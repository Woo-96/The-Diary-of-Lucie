#include "Lu_WindBreathState.h"
#include "Lu_EntScript.h"

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
	}

	void WindBreathState::CreateProjectile()
	{
	}
}
#include "Lu_PoisonBreathState.h"
#include "Lu_EntScript.h"

namespace Lu
{
	PoisonBreathState::PoisonBreathState()
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
	}

	void PoisonBreathState::Enter()
	{
		EntAttackState::Enter();
	}

	void PoisonBreathState::Exit()
	{
		EntAttackState::Exit();
	}

	void PoisonBreathState::ChangeAnimation()
	{
	}

	void PoisonBreathState::CreateProjectile()
	{
	}
}
#include "Lu_ThornState.h"
#include "Lu_EntScript.h"

namespace Lu
{
	ThornState::ThornState()
	{
		SetName(L"ThornStateScript");
		SetStateType(eState::Attack_Thorn);
	}

	ThornState::~ThornState()
	{
	}

	void ThornState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::Thorn);
	}

	void ThornState::Update()
	{
	}

	void ThornState::Enter()
	{
		EntAttackState::Enter();
	}

	void ThornState::Exit()
	{
		EntAttackState::Exit();
	}

	void ThornState::ChangeAnimation()
	{
	}

	void ThornState::CreateProjectile()
	{
	}
}
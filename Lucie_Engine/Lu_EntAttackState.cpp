#include "Lu_EntAttackState.h"
#include "Lu_EntScript.h"

namespace Lu
{
	EntAttackState::EntAttackState()
		: m_AttackCoolTime(0.f)
	{
		SetName(L"EntAttackStateScript");
	}

	EntAttackState::~EntAttackState()
	{
	}

	void EntAttackState::Initialize()
	{
		EntStateScript::Initialize();
	}

	void EntAttackState::Enter()
	{
		ChangeAnimation();
	}

	void EntAttackState::Exit()
	{
		SetTime(0.f);
	}
}

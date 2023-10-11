#include "Lu_EntIdleState.h"
#include "Lu_EntScript.h"

namespace Lu
{
	EntIdleState::EntIdleState()
	{
		SetName(L"EntIdleStateScript");
		SetStateType(eState::Idle);
	}

	EntIdleState::~EntIdleState()
	{
	}

	void EntIdleState::Update()
	{
		if(GetEntScript()->IsPatternStart())
			RandomAttack(3.f);
	}

	void EntIdleState::Enter()
	{

	}

	void EntIdleState::Exit()
	{
		SetTime(0.f);
	}
}
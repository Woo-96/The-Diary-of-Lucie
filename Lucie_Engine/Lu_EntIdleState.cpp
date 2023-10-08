#include "Lu_EntIdleState.h"

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
	}

	void EntIdleState::Enter()
	{
	}

	void EntIdleState::Exit()
	{
	}
}
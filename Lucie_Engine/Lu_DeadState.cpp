#include "Lu_DeadState.h"

namespace Lu
{
	DeadState::DeadState()
	{
		SetName(L"DeadStateScript");
		SetStateType(eState::Dead);
	}

	DeadState::~DeadState()
	{

	}

	void DeadState::Enter()
	{

	}

	void DeadState::Exit()
	{

	}
}
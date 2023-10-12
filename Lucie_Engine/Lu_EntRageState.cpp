#include "Lu_EntRageState.h"

namespace Lu
{
	EntRageState::EntRageState()
	{
		SetName(L"EntIdleStateScript");
		SetStateType(eState::Rage);
	}

	EntRageState::~EntRageState()
	{
	}

	void EntRageState::Enter()
	{
	}

	void EntRageState::Exit()
	{
	}
}
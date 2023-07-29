#include "Lu_SnabyDeadState.h"
#include "Lu_Object.h"
#include "Lu_SnabyScript.h"

namespace Lu
{
	SnabyDeadState::SnabyDeadState()
	{
		SetName(L"SnabyDeadStateScript");
		SetStateType(eState::Dead);
	}

	SnabyDeadState::~SnabyDeadState()
	{

	}

	void SnabyDeadState::Enter()
	{

	}

	void SnabyDeadState::Exit()
	{
		object::Destroy(GetSnabyScript()->GetOwner());
	}
}
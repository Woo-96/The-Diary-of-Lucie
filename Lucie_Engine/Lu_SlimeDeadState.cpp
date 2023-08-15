#include "Lu_SlimeDeadState.h"
#include "Lu_Object.h"
#include "Lu_SlimeScript.h"

namespace Lu
{
	SlimeDeadState::SlimeDeadState()
	{
		SetName(L"SlimeDeadStateScript");
		SetStateType(eState::Dead);
	}

	SlimeDeadState::~SlimeDeadState()
	{

	}

	void SlimeDeadState::Enter()
	{
		if (!(GetSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void SlimeDeadState::Exit()
	{
		object::Destroy(GetSlimeScript()->GetOwner());
	}
}
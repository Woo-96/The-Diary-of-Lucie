#include "Lu_HitState.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	HitState::HitState()
	{
		SetName(L"HitStateScript");
		SetStateType(eState::Hit);
	}

	HitState::~HitState()
	{

	}

	void HitState::Enter()
	{
		GetPlayerScript()->GetPlayerInfo().HP -= 1;
	}

	void HitState::Exit()
	{

	}
}
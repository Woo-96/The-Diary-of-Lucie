#include "Lu_KingSlimeIdleState.h"

namespace Lu
{
	KingSlimeIdleState::KingSlimeIdleState()
	{
		SetName(L"KingSlimeIdleStateScript");
		SetStateType(eState::Idle);
	}

	KingSlimeIdleState::~KingSlimeIdleState()
	{

	}

	void KingSlimeIdleState::Update()
	{
		// 아이들 -> 트레이스
		ChangeStateAfterTime(3.f, eState::Trace);
	}

	void KingSlimeIdleState::Enter()
	{

	}

	void KingSlimeIdleState::Exit()
	{
		SetTime(0.f);
	}
}
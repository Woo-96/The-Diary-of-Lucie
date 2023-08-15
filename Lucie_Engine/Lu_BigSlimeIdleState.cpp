#include "Lu_BigSlimeIdleState.h"

namespace Lu
{
	BigSlimeIdleState::BigSlimeIdleState()
	{
		SetName(L"BigSlimeIdleStateScript");
		SetStateType(eState::Idle);
	}

	BigSlimeIdleState::~BigSlimeIdleState()
	{

	}

	void BigSlimeIdleState::Update()
	{
		// 아이들 -> 트레이스
		ChangeStateAfterTime(3.f, eState::Trace);
	}

	void BigSlimeIdleState::Enter()
	{

	}

	void BigSlimeIdleState::Exit()
	{
		SetTime(0.f);
	}
}
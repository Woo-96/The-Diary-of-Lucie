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
		// ���̵� -> Ʈ���̽�
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
#include "Lu_SnabyIdleState.h"
#include "Lu_Time.h"
#include "Lu_SnabyScript.h"

namespace Lu
{
	SnabyIdleState::SnabyIdleState()
	{
		SetName(L"SnabyIdleStateScript");
		SetStateType(eState::Idle);
	}

	SnabyIdleState::~SnabyIdleState()
	{

	}

	void SnabyIdleState::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 5.f)
		{
			SetTime(0.f);
			GetSnabyScript()->ChangeState(eState::Patrol);
		}
	}

	void SnabyIdleState::Enter()
	{

	}

	void SnabyIdleState::Exit()
	{

	}
}
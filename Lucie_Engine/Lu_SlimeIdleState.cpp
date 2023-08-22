#include "Lu_SlimeIdleState.h"
#include "Lu_Time.h"
#include "Lu_SlimeScript.h"
#include "Lu_PlayerScript.h"
#include "Lu_GameObject.h"

namespace Lu
{
	SlimeIdleState::SlimeIdleState()
	{
		SetName(L"SlimeIdleStateScript");
		SetStateType(eState::Idle);
	}

	SlimeIdleState::~SlimeIdleState()
	{

	}

	void SlimeIdleState::Update()
	{
		// ���̵� -> ��Ʈ��
		ChangeStateAfterTime(2.f, eState::Patrol);


		// ���̵� -> Ʈ���̽�
		if (CalDirToPlayer().Length() < GetSlimeScript()->GetInfo().DetectRange)
		{
			GetSlimeScript()->ChangeState(eState::Trace);
		}
	}

	void SlimeIdleState::Enter()
	{

	}

	void SlimeIdleState::Exit()
	{
		SetTime(0.f);
	}
}
#include "Lu_SnabyIdleState.h"
#include "Lu_Time.h"
#include "Lu_SnabyScript.h"
#include "Lu_PlayerScript.h"
#include "Lu_GameObject.h"

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
		// ���̵� -> ��Ʈ��
		ChangeStateAfterTime(2.f, eState::Patrol);


		// ���̵� -> ����
		if (CalDirToPlayer().Length() < GetSnabyScript()->GetInfo().DetectRange)
		{
			GetSnabyScript()->ChangeState(eState::Attack);
		}
	}

	void SnabyIdleState::Enter()
	{

	}

	void SnabyIdleState::Exit()
	{
		SetTime(0.f);
	}
}
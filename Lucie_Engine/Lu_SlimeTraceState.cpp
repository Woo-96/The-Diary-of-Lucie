#include "Lu_SlimeTraceState.h"
#include "Lu_Time.h"
#include "Lu_SlimeScript.h"

namespace Lu
{
	SlimeTraceState::SlimeTraceState()
	{
		SetName(L"SlimeTraceStateScript");
		SetStateType(eState::Trace);
	}
	SlimeTraceState::~SlimeTraceState()
	{
	}

	void SlimeTraceState::Update()
	{
		// 추격
		Vector3 vPos = GetTransform()->GetPosition();
		float distanceToMove = GetSlimeScript()->GetInfo().MoveSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);

		// 추격 범위를 벗어남
		if (CalDirToPlayer().Length() > GetSlimeScript()->GetInfo().DetectRange)
		{
			GetSlimeScript()->ChangeState(eState::Idle);
		}

		// 공격 범위에 들어옴
		if (CalDirToPlayer().Length() < GetSlimeScript()->GetInfo().AttackRange)
		{
			int iAttackRand = std::rand() % 2;
			if(0 == iAttackRand)
				GetSlimeScript()->ChangeState(eState::Attack);
			else
				GetSlimeScript()->ChangeState(eState::JumpAttack);
		}
	}

	void SlimeTraceState::Enter()
	{
		// 방향 결정
		Vector3 vDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vDir.Normalize();
		SetDir(vDir);

		// 애니메이션 방향 결정
		DetermineAnimDir(vDir);
	}

	void SlimeTraceState::Exit()
	{
		SetTime(0.f);
	}
}
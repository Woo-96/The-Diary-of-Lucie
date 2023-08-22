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
		// �߰�
		Vector3 vPos = GetTransform()->GetPosition();
		float distanceToMove = GetSlimeScript()->GetInfo().MoveSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);

		// �߰� ������ ���
		if (CalDirToPlayer().Length() > GetSlimeScript()->GetInfo().DetectRange)
		{
			GetSlimeScript()->ChangeState(eState::Idle);
		}

		// ���� ������ ����
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
		// ���� ����
		Vector3 vDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vDir.Normalize();
		SetDir(vDir);

		// �ִϸ��̼� ���� ����
		DetermineAnimDir(vDir);
	}

	void SlimeTraceState::Exit()
	{
		SetTime(0.f);
	}
}
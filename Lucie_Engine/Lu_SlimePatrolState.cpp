#include "Lu_SlimePatrolState.h"
#include "Lu_Time.h"
#include "Lu_SlimeScript.h"
#include <cstdlib>
#include "Lu_PlayerScript.h"
#include "Lu_GameObject.h"

namespace Lu
{
	SlimePatrolState::SlimePatrolState()
	{
		SetName(L"SlimePatrolStateScript");
		SetStateType(eState::Patrol);
	}
	SlimePatrolState::~SlimePatrolState()
	{
	}

	void SlimePatrolState::Update()
	{
		// ��Ʈ��
		Vector3 vPos = GetTransform()->GetPosition();
		float fSpeed = GetSlimeScript()->GetInfo().MoveSpeed;
		float distanceToMove = fSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);


		// ��Ʈ�� -> ���̵�
		ChangeStateAfterTime(1.f, eState::Idle);


		// ��Ʈ�� -> ����
		if (CalDirToPlayer().Length() < GetSlimeScript()->GetInfo().DetectRange)
		{
			GetSlimeScript()->ChangeState(eState::Attack);
		}
	}

	void SlimePatrolState::Enter()
	{
		// �ִϸ��̼� ���� ����
		int rand = std::rand();
		int iMoveAngle = std::rand() % 360;
		DetermineAnimDir((float)iMoveAngle);

		// ���� ���� ���
		float Radian = math::DegreeToRadian((float)iMoveAngle);

		float moveX = std::cos(Radian);
		float moveY = std::sin(Radian);

		Vector3 MoveDir = Vector3(moveX, moveY, 0.f);
		MoveDir.Normalize();

		SetDir(MoveDir);
	}

	void SlimePatrolState::Exit()
	{
		SetTime(0.f);
	}
}
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
		float Zpos = vPos.z;
		float fSpeed = GetSlimeScript()->GetInfo().MoveSpeed;
		vPos += GetDir() * fSpeed * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);


		// ��Ʈ�� -> ���̵�
		ChangeStateAfterTime(2.f, eState::Idle);


		// ��Ʈ�� -> Ʈ���̽�
		if (CalDirToPlayer().Length() < GetSlimeScript()->GetInfo().DetectRange)
		{
			GetSlimeScript()->ChangeState(eState::Trace);
		}
	}

	void SlimePatrolState::Enter()
	{
		// �ִϸ��̼� ���� ����
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
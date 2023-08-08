#include "Lu_SnabyPatrolState.h"
#include "Lu_Time.h"
#include "Lu_SnabyScript.h"
#include <cstdlib>
#include "Lu_PlayerScript.h"
#include "Lu_GameObject.h"

namespace Lu
{
	SnabyPatrolState::SnabyPatrolState()
	{
		SetName(L"SnabyPatrolStateScript");
		SetStateType(eState::Patrol);
	}
	SnabyPatrolState::~SnabyPatrolState()
	{
	}

	void SnabyPatrolState::Update()
	{
		// ��Ʈ��
		Vector3 vPos = GetTransform()->GetPosition();
		float fSpeed = GetSnabyScript()->GetInfo().MoveSpeed;
		float distanceToMove = fSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);


		// ��Ʈ�� -> ���̵�
		ChangeStateAfterTime(1.f, eState::Idle);


		// ��Ʈ�� -> ����
		if (CalDirToPlayer().Length() < GetSnabyScript()->GetInfo().DetectRange)
		{
			GetSnabyScript()->ChangeState(eState::Attack);
		}
	}

	void SnabyPatrolState::Enter()
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

	void SnabyPatrolState::Exit()
	{
		SetTime(0.f);
	}
}
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
		// 패트롤
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;
		float fSpeed = GetSlimeScript()->GetInfo().MoveSpeed;
		vPos += GetDir() * fSpeed * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);


		// 패트롤 -> 아이들
		ChangeStateAfterTime(2.f, eState::Idle);


		// 패트롤 -> 트레이스
		if (CalDirToPlayer().Length() < GetSlimeScript()->GetInfo().DetectRange)
		{
			GetSlimeScript()->ChangeState(eState::Trace);
		}
	}

	void SlimePatrolState::Enter()
	{
		// 애니메이션 방향 결정
		int iMoveAngle = std::rand() % 360;
		DetermineAnimDir((float)iMoveAngle);

		// 진행 방향 계산
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
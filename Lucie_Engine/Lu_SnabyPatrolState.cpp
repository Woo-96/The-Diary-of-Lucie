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
		// 패트롤
		Vector3 vPos = GetTransform()->GetPosition();
		float fSpeed = GetSnabyScript()->GetInfo().MoveSpeed;
		float distanceToMove = fSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);


		// 패트롤 -> 아이들
		ChangeStateAfterTime(1.f, eState::Idle);


		// 패트롤 -> 어택
		if (CalDirToPlayer().Length() < GetSnabyScript()->GetInfo().DetectRange)
		{
			GetSnabyScript()->ChangeState(eState::Attack);
		}
	}

	void SnabyPatrolState::Enter()
	{
		// 애니메이션 방향 결정
		int rand = std::rand();
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

	void SnabyPatrolState::Exit()
	{
		SetTime(0.f);
	}
}
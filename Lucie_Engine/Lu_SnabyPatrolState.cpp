#include "Lu_SnabyPatrolState.h"
#include "Lu_Time.h"
#include "Lu_SnabyScript.h"
#include <cstdlib>

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
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 1.5f)
		{
			SetTime(0.f);
			GetSnabyScript()->ChangeState(eState::Idle);
		}

		Vector3 vPos = GetTransform()->GetPosition();
		float fSpeed = GetSnabyScript()->GetInfo().MoveSpeed;

		float distanceToMove = fSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;

		vPos += moveVector;

		GetTransform()->SetPosition(vPos);
	}

	void SnabyPatrolState::Enter()
	{
		int iMoveAngle = std::rand() % 360;

		if (45 < iMoveAngle && iMoveAngle <= 135)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (135 < iMoveAngle && iMoveAngle <= 225)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else if (225 < iMoveAngle && iMoveAngle <= 315)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Down);
		else
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Right);

		float Radian = math::DegreeToRadian((float)iMoveAngle);

		float moveX = std::cos(Radian);
		float moveY = std::sin(Radian);

		Vector3 MoveDir = Vector3(moveX, moveY, 0.f);
		MoveDir.Normalize();

		SetDir(MoveDir);
	}

	void SnabyPatrolState::Exit()
	{

	}
}
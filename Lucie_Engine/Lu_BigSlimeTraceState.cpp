#include "Lu_BigSlimeTraceState.h"
#include "Lu_BigSlimeScript.h"
#include "Lu_Time.h"

namespace Lu
{
	BigSlimeTraceState::BigSlimeTraceState()
	{
		SetName(L"BigSlimeTraceStateScript");
		SetStateType(eState::Trace);
	}

	BigSlimeTraceState::~BigSlimeTraceState()
	{

	}

	void BigSlimeTraceState::Update()
	{
		// 플레이어 추적
		Vector3 vPos = GetTransform()->GetPosition();
		float fSpeed = GetBigSlimeScript()->GetInfo().MoveSpeed;
		Vector3 vMoveDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vMoveDir.Normalize();
		Vector3 vMove = vMoveDir * fSpeed * (float)Time::DeltaTime();
		vPos += vMove;
		GetTransform()->SetPosition(vPos);

		// 애니메이션 업데이트
		DetermineAnimDir(vMoveDir);

		// 트레이스 -> 어택
		ChangeStateAfterTime(10.f, eState::Attack);
	}

	void BigSlimeTraceState::Enter()
	{

	}

	void BigSlimeTraceState::Exit()
	{
		SetTime(0.f);
	}
}
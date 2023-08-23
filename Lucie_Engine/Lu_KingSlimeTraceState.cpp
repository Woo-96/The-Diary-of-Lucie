#include "Lu_KingSlimeTraceState.h"
#include "Lu_KingSlimeScript.h"
#include "Lu_Time.h"

namespace Lu
{
	KingSlimeTraceState::KingSlimeTraceState()
	{
		SetName(L"KingSlimeTraceStateScript");
		SetStateType(eState::Trace);
	}

	KingSlimeTraceState::~KingSlimeTraceState()
	{

	}

	void KingSlimeTraceState::Update()
	{
		// �÷��̾� ����
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;
		float fSpeed = GetKingSlimeScript()->GetInfo().MoveSpeed;
		Vector3 vMoveDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vMoveDir.Normalize();
		vPos += vMoveDir * fSpeed * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);

		// �ִϸ��̼� ������Ʈ
		DetermineAnimDir(vMoveDir);

		// Ʈ���̽� -> ����
		ChangeStateAfterTime(5.f, eState::Attack);
	}

	void KingSlimeTraceState::Enter()
	{

	}

	void KingSlimeTraceState::Exit()
	{
		SetTime(0.f);
	}
}
#include "Lu_DashState.h"
#include "Lu_Time.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	DashState::DashState()
	{
		SetName(L"DashStateScript");
		SetStateType(eState::Dash);
	}

	DashState::~DashState()
	{

	}

	void DashState::Initialize()
	{
		StateScript::Initialize();

		SetSpeed(400.f);
	}

	void DashState::Update()
	{
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;
		vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);
	}

	void DashState::Enter()
	{
		GetPlayerScript()->SetAction(true);
		CalDirToMouse();
	}

	void DashState::Exit()
	{
		GetPlayerScript()->SetAction(false);
	}
}
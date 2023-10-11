#include "Lu_CraterState.h"
#include "Lu_EntScript.h"

namespace Lu
{
	CraterState::CraterState()
	{
		SetName(L"CraterStateScript");
		SetStateType(eState::Attack_Crater);
	}

	CraterState::~CraterState()
	{
	}

	void CraterState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::Crater);
	}

	void CraterState::Update()
	{
	}

	void CraterState::Enter()
	{
		EntAttackState::Enter();
	}

	void CraterState::Exit()
	{
		EntAttackState::Exit();
	}

	void CraterState::ChangeAnimation()
	{
	}

	void CraterState::CreateProjectile()
	{
	}
}
#include "Lu_DeadState.h"
#include "Lu_Object.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	DeadState::DeadState()
	{
		SetName(L"DeadStateScript");
		SetStateType(eState::Dead);
	}

	DeadState::~DeadState()
	{

	}

	void DeadState::Enter()
	{

	}

	void DeadState::Exit()
	{
		object::Destroy(GetPlayerScript()->GetOwner());
		SceneManager::LoadScene(L"GameOverScene");
	}
}
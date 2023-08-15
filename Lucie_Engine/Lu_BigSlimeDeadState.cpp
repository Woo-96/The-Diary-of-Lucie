#include "Lu_BigSlimeDeadState.h"
#include "Lu_BigSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"

namespace Lu
{
	BigSlimeDeadState::BigSlimeDeadState()
	{
		SetName(L"BigSlimeDeadStateScript");
		SetStateType(eState::Dead);
	}

	BigSlimeDeadState::~BigSlimeDeadState()
	{

	}

	void BigSlimeDeadState::Update()
	{

	}

	void BigSlimeDeadState::Enter()
	{
		if (!(GetBigSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void BigSlimeDeadState::Exit()
	{
		object::Destroy(GetBigSlimeScript()->GetOwner());

		// 빅 슬라임 2마리 스폰

	}
}

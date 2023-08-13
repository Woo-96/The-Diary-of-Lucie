#include "Lu_KingSlimeDeadState.h"
#include "Lu_KingSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"

namespace Lu
{
	KingSlimeDeadState::KingSlimeDeadState()
	{
		SetName(L"KingSlimeDeadStateScript");
		SetStateType(eState::Dead);
	}

	KingSlimeDeadState::~KingSlimeDeadState()
	{

	}

	void KingSlimeDeadState::Update()
	{

	}

	void KingSlimeDeadState::Enter()
	{
		if (!(GetKingSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void KingSlimeDeadState::Exit()
	{
		object::Destroy(GetKingSlimeScript()->GetOwner());

		// 빅 슬라임 2마리 스폰

	}
}

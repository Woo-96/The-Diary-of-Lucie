#include "Lu_SnabyDeadState.h"
#include "Lu_Object.h"
#include "Lu_SnabyScript.h"
#include "Lu_Resources.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	SnabyDeadState::SnabyDeadState()
	{
		SetName(L"SnabyDeadStateScript");
		SetStateType(eState::Dead);
	}

	SnabyDeadState::~SnabyDeadState()
	{

	}

	void SnabyDeadState::Enter()
	{
		if (!(GetSnabyScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void SnabyDeadState::Exit()
	{
		GetTarget()->IncreaseEXP(30);

		object::Destroy(GetSnabyScript()->GetOwner());
	}
}
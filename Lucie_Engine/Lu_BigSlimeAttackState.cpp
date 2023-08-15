#include "Lu_BigSlimeAttackState.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_BubbleProjectile.h"

namespace Lu
{
	BigSlimeAttackState::BigSlimeAttackState()
	{
		SetName(L"BigSlimeAttackStateScript");
		SetStateType(eState::Attack);
	}

	BigSlimeAttackState::~BigSlimeAttackState()
	{

	}

	void BigSlimeAttackState::Enter()
	{

	}

	void BigSlimeAttackState::Exit()
	{
		
	}
}
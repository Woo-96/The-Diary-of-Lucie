#include "Lu_AttackState.h"
#include "Lu_Time.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	AttackState::AttackState()
	{
		SetName(L"AttackStateScript");
		SetStateType(eState::Attack);
	}

	AttackState::~AttackState()
	{

	}

	void AttackState::Initialize()
	{
		StateScript::Initialize();

		SetSpeed(50.f);
	}

	void AttackState::Update()
	{
		Vector3 vPos = GetTransform()->GetPosition();

		float distanceToMove = GetSpeed() * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;

		vPos += moveVector;

		GetTransform()->SetPosition(vPos);


		//switch (m_CurWeapon)
		//{
		//case Lu::PlayerScript::eWeaponType::Sword:
		//	break;
		//case Lu::PlayerScript::eWeaponType::Bow:
		//	break;
		//case Lu::PlayerScript::eWeaponType::Wand:
		//	break;
		//}
	}

	void AttackState::Enter()
	{
		GetOwner()->SetAction(true);
		CalDirToMouse();
	}

	void AttackState::Exit()
	{
		GetOwner()->SetAction(false);
	}
}
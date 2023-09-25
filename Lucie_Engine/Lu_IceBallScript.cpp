#include "Lu_IceBallScript.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"

namespace Lu
{
	IceBallScript::IceBallScript()
	{
		SetName(L"IceBallScript");
		SetSkillType(PlayerScript::eSkillType::IceBall);
	}

	IceBallScript::~IceBallScript()
	{
	}

	void IceBallScript::Update()
	{
		ProjectileScript::Update();
	}

	void IceBallScript::OnCollisionEnter(Collider2D* _Other)
	{
		ProjectileScript::OnCollisionEnter(_Other);
	}
}

#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeAttackState : public SlimeStateScript
	{
	public:
		SlimeAttackState();
		virtual ~SlimeAttackState();

	private:
		Vector3 m_AttackDir;
		GameObject* m_SFX;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
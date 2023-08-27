#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeJumpAttackState : public SlimeStateScript
	{
	public:
		SlimeJumpAttackState();
		virtual ~SlimeJumpAttackState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
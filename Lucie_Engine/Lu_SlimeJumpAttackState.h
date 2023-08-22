#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeJumpAttackState : public SlimeStateScript
	{
	public:
		SlimeJumpAttackState();
		virtual ~SlimeJumpAttackState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeAttackState : public KingSlimeStateScript
	{
	public:
		KingSlimeAttackState();
		virtual ~KingSlimeAttackState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
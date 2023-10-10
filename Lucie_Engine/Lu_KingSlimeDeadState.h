#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeDeadState : public KingSlimeStateScript
	{
	public:
		KingSlimeDeadState();
		virtual ~KingSlimeDeadState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeAttackBubbleState : public KingSlimeStateScript
	{
	public:
		KingSlimeAttackBubbleState();
		virtual ~KingSlimeAttackBubbleState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
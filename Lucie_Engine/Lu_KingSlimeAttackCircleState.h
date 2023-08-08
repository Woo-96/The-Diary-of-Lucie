#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeAttackCircleState : public KingSlimeStateScript
	{
	public:
		KingSlimeAttackCircleState();
		virtual ~KingSlimeAttackCircleState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
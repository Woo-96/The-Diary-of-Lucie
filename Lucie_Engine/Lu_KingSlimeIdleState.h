#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeIdleState : public KingSlimeStateScript
	{
	public:
		KingSlimeIdleState();
		virtual ~KingSlimeIdleState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

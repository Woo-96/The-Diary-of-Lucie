#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeIdleState : public SlimeStateScript
	{
	public:
		SlimeIdleState();
		virtual ~SlimeIdleState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

#pragma once
#include "Lu_BigSlimeStateScript.h"

namespace Lu
{
	class BigSlimeDeadState : public BigSlimeStateScript
	{
	public:
		BigSlimeDeadState();
		virtual ~BigSlimeDeadState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
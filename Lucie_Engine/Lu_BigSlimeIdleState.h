#pragma once
#include "Lu_BigSlimeStateScript.h"

namespace Lu
{
	class BigSlimeIdleState : public BigSlimeStateScript
	{
	public:
		BigSlimeIdleState();
		virtual ~BigSlimeIdleState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

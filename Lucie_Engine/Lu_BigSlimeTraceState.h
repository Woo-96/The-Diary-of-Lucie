#pragma once
#include "Lu_BigSlimeStateScript.h"

namespace Lu
{
	class BigSlimeTraceState : public BigSlimeStateScript
	{
	public:
		BigSlimeTraceState();
		virtual ~BigSlimeTraceState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeTraceState : public SlimeStateScript
	{
	public:
		SlimeTraceState();
		virtual ~SlimeTraceState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
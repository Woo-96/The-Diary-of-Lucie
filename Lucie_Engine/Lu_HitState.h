#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class HitState : public StateScript
	{
	public:
		HitState();
		virtual ~HitState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
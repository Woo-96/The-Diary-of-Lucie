#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class DeadState : public StateScript
	{
	public:
		DeadState();
		virtual ~DeadState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
#pragma once
#include "Lu_EntStateScript.h"

namespace Lu
{
	class EntDeadState : public EntStateScript
	{
	public:
		EntDeadState();
		virtual ~EntDeadState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
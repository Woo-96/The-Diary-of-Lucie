#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class IdleState : public StateScript
	{
	public:
		IdleState();
		virtual ~IdleState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

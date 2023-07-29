#pragma once
#include "Lu_SnabyStateScript.h"

namespace Lu
{
	class SnabyIdleState : public SnabyStateScript
	{
	public:
		SnabyIdleState();
		virtual ~SnabyIdleState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

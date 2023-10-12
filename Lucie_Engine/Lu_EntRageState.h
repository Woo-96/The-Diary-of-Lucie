#pragma once
#include "Lu_EntStateScript.h"

namespace Lu
{
	class EntRageState : public EntStateScript
	{
	public:
		EntRageState();
		virtual ~EntRageState();

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

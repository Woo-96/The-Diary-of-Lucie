#pragma once
#include "Lu_EntStateScript.h"

namespace Lu
{
	class EntIdleState : public EntStateScript
	{
	public:
		EntIdleState();
		virtual ~EntIdleState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

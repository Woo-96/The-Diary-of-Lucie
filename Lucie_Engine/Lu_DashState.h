#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class DashState : public StateScript
	{
	public:
		DashState();
		virtual ~DashState();

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
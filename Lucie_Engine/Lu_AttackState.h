#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class AttackState : public StateScript
	{
	public:
		AttackState();
		virtual ~AttackState();

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
#pragma once
#include "Lu_EntStateScript.h"

namespace Lu
{
	class EntAttackState : public EntStateScript
	{
	public:
		EntAttackState();
		virtual ~EntAttackState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class MoveState : public StateScript
	{
	public:
		MoveState();
		virtual ~MoveState();

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
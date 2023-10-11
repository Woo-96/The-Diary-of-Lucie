#pragma once
#include "Lu_EntAttackState.h"

namespace Lu
{
	class TomatoBoomState : public EntAttackState
	{
	public:
		TomatoBoomState();
		virtual ~TomatoBoomState();

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;

	protected:
		virtual void ChangeAnimation() override;
		virtual void CreateProjectile() override;
	};
}
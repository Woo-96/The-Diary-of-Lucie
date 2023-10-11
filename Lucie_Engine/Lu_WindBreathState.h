#pragma once
#include "Lu_EntAttackState.h"

namespace Lu
{
	class WindBreathState : public EntAttackState
	{
	public:
		WindBreathState();
		virtual ~WindBreathState();

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
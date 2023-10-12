#pragma once
#include "Lu_EntAttackState.h"

namespace Lu
{
	class TomatoBombState : public EntAttackState
	{
	public:
		TomatoBombState();
		virtual ~TomatoBombState();

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
#pragma once
#include "Lu_EntAttackState.h"

namespace Lu
{
	class CraterState : public EntAttackState
	{
	public:
		CraterState();
		virtual ~CraterState();

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
#pragma once
#include "Lu_EntAttackState.h"

namespace Lu
{
	class ThornState : public EntAttackState
	{
	public:
		ThornState();
		virtual ~ThornState();

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
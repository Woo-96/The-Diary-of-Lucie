#pragma once
#include "Lu_SnabyStateScript.h"

namespace Lu
{
	class SnabyAttackState : public SnabyStateScript
	{
	public:
		SnabyAttackState();
		virtual ~SnabyAttackState();

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;

	private:
		void CreateProjectile();
	};
}
#pragma once
#include "Lu_SnabyStateScript.h"

namespace Lu
{
	class SnabyAttackState : public SnabyStateScript
	{
	public:
		SnabyAttackState();
		virtual ~SnabyAttackState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;

	private:
		void CreateProjectile(Vector3 _Dir);
	};
}
#pragma once
#include "Lu_EntStateScript.h"

namespace Lu
{
	class EntAttackState : public EntStateScript
	{
	public:
		EntAttackState();
		virtual ~EntAttackState();

	private:
		float	m_AttackCoolTime;

	public:
		void SetAttackCoolTime(float _AttackCoolTime)
		{
			m_AttackCoolTime = _AttackCoolTime;
		}

	public:
		float GetAttackCoolTime()	const
		{
			return m_AttackCoolTime;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override {}

	public:
		virtual void Enter() override;
		virtual void Exit() override;

	protected:
		virtual void ChangeAnimation() = 0;
		virtual void CreateProjectile() = 0;
	};
}
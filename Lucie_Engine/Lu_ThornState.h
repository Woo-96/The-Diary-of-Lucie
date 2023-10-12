#pragma once
#include "Lu_EntAttackState.h"

namespace Lu
{
	class ThornState : public EntAttackState
	{
	public:
		ThornState();
		virtual ~ThornState();

	private:
		bool		m_bFirst;
		GameObject* m_FX;

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;

	protected:
		virtual void ChangeAnimation() override;
		virtual void CreateProjectile() override;

	private:
		void StartFX();
		void FinishedFX();
	};
}
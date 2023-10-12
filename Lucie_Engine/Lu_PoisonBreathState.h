#pragma once
#include "Lu_EntAttackState.h"

namespace Lu
{
	class PoisonBreathState : public EntAttackState
	{
	public:
		enum class ePoisonDir
		{
			Left,
			Right,
		};

	public:
		PoisonBreathState();
		virtual ~PoisonBreathState();

	private:
		ePoisonDir	m_StartDir;

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
#pragma once
#include "Lu_BigSlimeStateScript.h"

namespace Lu
{
	class BigSlimeAttackState : public BigSlimeStateScript
	{
	public:
		BigSlimeAttackState();
		virtual ~BigSlimeAttackState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
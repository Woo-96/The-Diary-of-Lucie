#pragma once
#include "Lu_BigSlimeStateScript.h"

namespace Lu
{
	class BigSlimeDeadState : public BigSlimeStateScript
	{
	public:
		BigSlimeDeadState();
		virtual ~BigSlimeDeadState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
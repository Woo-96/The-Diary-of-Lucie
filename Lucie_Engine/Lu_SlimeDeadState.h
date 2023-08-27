#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeDeadState : public SlimeStateScript
	{
	public:
		SlimeDeadState();
		virtual ~SlimeDeadState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
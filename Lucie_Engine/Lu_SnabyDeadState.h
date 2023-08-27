#pragma once
#include "Lu_SnabyStateScript.h"

namespace Lu
{
	class SnabyDeadState : public SnabyStateScript
	{
	public:
		SnabyDeadState();
		virtual ~SnabyDeadState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
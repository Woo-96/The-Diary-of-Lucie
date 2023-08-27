#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeDeadState : public KingSlimeStateScript
	{
	public:
		KingSlimeDeadState();
		virtual ~KingSlimeDeadState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
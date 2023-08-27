#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeAttackState : public KingSlimeStateScript
	{
	public:
		KingSlimeAttackState();
		virtual ~KingSlimeAttackState();

	private:
		GameObject* m_SFX;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
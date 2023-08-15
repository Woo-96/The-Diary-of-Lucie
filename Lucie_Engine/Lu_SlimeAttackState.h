#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeAttackState : public SlimeStateScript
	{
	public:
		SlimeAttackState();
		virtual ~SlimeAttackState();

	private:
		float m_ChangeDist;

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;

	private:
		void CreateProjectile(Vector3 _Dir);
	};
}
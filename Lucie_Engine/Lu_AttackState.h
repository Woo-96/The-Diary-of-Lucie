#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class GameObject;
	class AttackState : public StateScript
	{
	public:
		AttackState();
		virtual ~AttackState();

	private:
		GameObject* m_AttackSFX;

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;

	private:
		void CreateProjectile();
	};
}
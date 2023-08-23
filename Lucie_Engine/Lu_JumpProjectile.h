#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
	class Animator;
	class JumpProjectile : public MonsterProjectileScript
	{
	public:
		JumpProjectile();
		virtual ~JumpProjectile();

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override;
		virtual void OnCollisionExit(Collider2D* other) override;
	};
}



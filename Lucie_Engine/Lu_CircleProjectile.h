#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
	class CircleProjectile : public MonsterProjectileScript
	{
	public:
		CircleProjectile();
		virtual ~CircleProjectile();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;
	};
}



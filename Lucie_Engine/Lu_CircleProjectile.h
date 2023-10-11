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
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;

    protected:
        virtual void CreateProjectileAnimation() override {}
	};
}

#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class TraceCircleProjectile : public MonsterProjectileScript
    {
    public:
        TraceCircleProjectile();
        virtual ~TraceCircleProjectile();

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;

    protected:
        virtual void CreateProjectileAnimation() override {}
    };
}

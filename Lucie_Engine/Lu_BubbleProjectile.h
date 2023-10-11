#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class BubbleProjectile : public MonsterProjectileScript
    {
    public:
        BubbleProjectile();
        virtual ~BubbleProjectile();

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;

    protected:
        virtual void CreateProjectileAnimation() override {}
    };
}
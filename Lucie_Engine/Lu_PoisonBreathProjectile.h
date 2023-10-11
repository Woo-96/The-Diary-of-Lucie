#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class PoisonBreathProjectile : public MonsterProjectileScript
    {
    public:
        PoisonBreathProjectile();
        virtual ~PoisonBreathProjectile();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    protected:
        virtual void CreateProjectileAnimation() override;
    };
}
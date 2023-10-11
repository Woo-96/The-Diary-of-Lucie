#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class WindBreathProjectile : public MonsterProjectileScript
    {
    public:
        WindBreathProjectile();
        virtual ~WindBreathProjectile();

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
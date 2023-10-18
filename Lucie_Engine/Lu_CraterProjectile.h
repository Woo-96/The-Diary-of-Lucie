#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class CraterProjectile : public MonsterProjectileScript
    {
    public:
        CraterProjectile();
        virtual ~CraterProjectile();

    private:
        bool    m_bFirst;

    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;

    protected:
        virtual void CreateProjectileAnimation() override;

    private:
        void AnimationComplete();
    };
}

#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class ThornProjectile : public MonsterProjectileScript
    {
    public:
        ThornProjectile();
        virtual ~ThornProjectile();

    private:
        bool    m_bFirst;

    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override {}

    protected:
        virtual void CreateProjectileAnimation() override;

    private:
        void ThornComplete();
    };
}
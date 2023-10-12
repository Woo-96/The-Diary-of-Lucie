#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class TomatoBombProjectile : public MonsterProjectileScript
    {
    public:
        TomatoBombProjectile();
        virtual ~TomatoBombProjectile();

    private:
        int     m_HP;
        float   m_HitCoolTime;
        Vector2 m_Random;

    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;

    protected:
        virtual void CreateProjectileAnimation() override;

    private:
        void InflictDamage(int _Damage);
        void Bomb();
    };
}
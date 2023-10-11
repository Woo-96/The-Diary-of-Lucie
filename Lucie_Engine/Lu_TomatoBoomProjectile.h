#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class TomatoBoomProjectile : public MonsterProjectileScript
    {
    public:
        TomatoBoomProjectile();
        virtual ~TomatoBoomProjectile();

    private:
        int     m_HP;
        float   m_HitCoolTime;

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
        void Boom();
    };
}
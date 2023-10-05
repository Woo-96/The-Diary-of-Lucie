#pragma once
#include "Lu_ProjectileScript.h"

namespace Lu
{
    class WandProjectile : public ProjectileScript
    {
    public:
        WandProjectile();
        virtual ~WandProjectile();

    private:
        bool    m_bChargeProjectile;
        bool    m_bFullCharge;
        float   m_SFXTime;

    public:
        void SetChargeProjectile(bool _b)
        {
            m_bChargeProjectile = _b;
        }

        void SetFullCharge(bool _b)
        {
            m_bFullCharge = _b;
        }

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override;
        virtual void OnCollisionStay(Collider2D* _Other) override;

    public:
        virtual int GetProjectileDamage() override;
    };
}
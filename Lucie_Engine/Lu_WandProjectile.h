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
        bool m_bChargeProjectile;

    public:
        void SetChargeProjectile(bool _b)
        {
            m_bChargeProjectile = _b;
        }

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;
    };
}
#pragma once
#include "Lu_ProjectileScript.h"

namespace Lu
{
    class BowProjectile : public ProjectileScript
    {
    public:
        BowProjectile();
        virtual ~BowProjectile();

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;
    };
}
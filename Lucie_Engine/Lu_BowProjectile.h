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
        virtual void OnCollisionEnter(Collider2D* _Other) override;

    public:
        virtual int GetProjectileDamage() override;
    };
}
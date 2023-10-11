#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class ThornProjectile : public MonsterProjectileScript
    {
    public:
        ThornProjectile();
        virtual ~ThornProjectile();

    public:
        virtual void Initialize() override;

    protected:
        virtual void CreateProjectileAnimation() override;

    private:
        void ThornComplete();
    };
}
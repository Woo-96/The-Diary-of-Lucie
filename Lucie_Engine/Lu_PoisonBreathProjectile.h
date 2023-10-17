#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class PoisonBreathProjectile : public MonsterProjectileScript
    {
    public:
        PoisonBreathProjectile();
        virtual ~PoisonBreathProjectile();

    private:
        float   m_Degree;

    public:
        void SetDegree(float _Degree)
        {
            m_Degree = _Degree;
        }

    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override {}

    protected:
        virtual void CreateProjectileAnimation() override;
    };
}
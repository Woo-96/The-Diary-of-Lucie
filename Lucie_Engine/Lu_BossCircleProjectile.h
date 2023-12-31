#pragma once
#include "Lu_MonsterProjectileScript.h"

namespace Lu
{
    class BossCircleProjectile : public MonsterProjectileScript
    {
    public:
        BossCircleProjectile();
        virtual ~BossCircleProjectile();

    private:
        Vector3 m_CenterPos;

    public:
        void SetCenterPos(Vector3 _CenterPos)
        {
            m_CenterPos = _CenterPos;
        }

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;

    protected:
        virtual void CreateProjectileAnimation() override {}
    };
}
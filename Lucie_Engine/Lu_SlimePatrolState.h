#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
    class SlimePatrolState : public SlimeStateScript
    {
    public:
        SlimePatrolState();
        virtual ~SlimePatrolState();

    public:
        virtual void Update() override;

    public:
        virtual void Enter() override;
        virtual void Exit() override;
    };
}

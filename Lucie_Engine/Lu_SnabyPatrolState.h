#pragma once
#include "Lu_SnabyStateScript.h"

namespace Lu
{
    class SnabyPatrolState : public SnabyStateScript
    {
    public:
        SnabyPatrolState();
        virtual ~SnabyPatrolState();

    public:
        virtual void Enter() override;
        virtual void Exit() override;
    };
}

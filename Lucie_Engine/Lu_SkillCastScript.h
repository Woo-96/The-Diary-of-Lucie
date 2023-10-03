#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
    class Animator;
    class SkillCastScript : public Script
    {
    public:
        SkillCastScript();
        virtual ~SkillCastScript();

    private:
        Animator*   m_Animator;
        Vector3     m_SpawnPos;

    public:
        void SetSpawnPos(Vector3 _SpawnPos)
        {
            m_SpawnPos = _SpawnPos;
        }

    public:
        virtual void Initialize() override;
    
    private:
        void CompleteAnimation();
    };
}

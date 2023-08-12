#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
    class Animator;
    class MonsterScript : public Script
    {
    public:
        struct tMonsterInfo
        {
            int     HP;
            int     MaxHP;
            float   DetectRange;
            float   MoveSpeed;
        };

        enum class eAnimDir
        {
            Left,
            Right,
            Up,
            Down,
        };

    public:
        MonsterScript();
        virtual ~MonsterScript();

    private:
        Animator*       m_Animator;
        tMonsterInfo    m_MonsterInfo;
        eAnimDir		m_AnimDir;
        eAnimDir		m_PrevAnimDir;

    public:
        void SetCurDir(eAnimDir _Dir)
        {
            m_AnimDir = _Dir;
        }

    public:
        Animator* GetAnimator()
        {
            return m_Animator;
        }

        tMonsterInfo& GetInfo()
        {
            return m_MonsterInfo;
        }

        eAnimDir GetCurDir()    const
        {
            return m_AnimDir;
        }

        eAnimDir GetPrevDir()   const
        {
            return m_PrevAnimDir;
        }
        
    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override {}
        virtual void OnCollisionStay(Collider2D* _Other) override {}
        virtual void OnCollisionExit(Collider2D* _Other) override {}

    protected:
        virtual void CreateAnimation() {}
        virtual void AnimationUpdate() {}
    };
}


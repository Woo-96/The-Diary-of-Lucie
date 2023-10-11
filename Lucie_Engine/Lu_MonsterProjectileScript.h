#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_MonsterScript.h"

namespace Lu
{
    class Transform;
    class Animator;
    class MonsterProjectileScript : public Script
    {
    public:
        MonsterProjectileScript();
        virtual ~MonsterProjectileScript();

    private:
        MonsterScript*  m_MonsterScript;
        Transform*      m_Transform;
        Animator*       m_Animator;
        float		    m_AttackSpeed;
        Vector3		    m_AttackDir;
        float           m_Time;
        float           m_Duration;

    public:
        void SetMonsterScript(MonsterScript* _MonsterScript)
        {
            m_MonsterScript = _MonsterScript;
        }

        void SetTransform(Transform* _Transform)
        {
            m_Transform = _Transform;
        }

        void SetSpeed(float _Speed)
        {
            m_AttackSpeed = _Speed;
        }

        void SetDir(Vector3 _Dir)
        {
            m_AttackDir = _Dir;
        }

        void SetTime(float _Time)
        {
            m_Time = _Time;
        }

        void SetDuration(float _Duration)
        {
            m_Duration = _Duration;
        }

    public:
        MonsterScript* GetMonsterScript() const
        {
            return m_MonsterScript;
        }

        Transform* GetTransform()   const
        {
            return m_Transform;
        }

        Animator* GetAnimator() const
        {
            return m_Animator;
        }

        float GetSpeed()    const
        {
            return m_AttackSpeed;
        }

        Vector3 GetDir()    const
        {
            return m_AttackDir;
        }

        float GetTime() const
        {
            return m_Time;
        }

        float GetDuration() const
        {
            return m_Duration;
        }

    public:
        virtual void Initialize() override;
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override;
        virtual void OnCollisionStay(Collider2D* _Other) override {}
        virtual void OnCollisionExit(Collider2D* _Other) override {}

    protected:
        virtual void CreateProjectileAnimation() = 0;
    };
}
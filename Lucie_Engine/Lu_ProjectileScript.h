#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_ScriptEnums.h"

namespace Lu
{
    class GameObject;
    class Transform;
    class Animator;
    class ProjectileScript : public Script
    {
    public:
        ProjectileScript();
        virtual ~ProjectileScript();

    private:
        Transform*                  m_Transform;
        float			            m_AttackSpeed;
        Vector3			            m_AttackDir;
        float                       m_Time;
        float                       m_Duration;

        // 무기 투사체 전용
        eWeaponType                 m_WeaponType;

        // 스킬 투사체 전용
        eSkillType                  m_SkillType;

    public:
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

        void SetWeaponType(eWeaponType _Type)
        {
            m_WeaponType = _Type;
        }

        void SetSkillType(eSkillType _SkillType)
        {
            m_SkillType = _SkillType;
        }

    public:
        float GetTime() const
        {
            return m_Time;
        }

        float GetDuration() const
        {
            return m_Duration;
        }

    public:		
        virtual void Initialize() override {};
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override;
        virtual void OnCollisionStay(Collider2D* _Other) override {}
        virtual void OnCollisionExit(Collider2D* _Other) override {}
    };
}
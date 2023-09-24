#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
    class GameObject;
    class Transform;
    class ProjectileScript : public Script
    {
    public:
        ProjectileScript();
        virtual ~ProjectileScript();

    private:
        PlayerScript*   m_PlayerScript;
        Transform*      m_Transform;
        eWeaponType     m_ProjectileType;
        float			m_AttackSpeed;
        Vector3			m_AttackDir;
        float           m_Time;
        float           m_Duration;

    public:
        void SetPlayerScript(PlayerScript* _PlayerScript)
        {
            m_PlayerScript = _PlayerScript;
        }

        void SetTransform(Transform* _Transform)
        {
            m_Transform = _Transform;
        }

        void SetType(eWeaponType _Type)
        {
            m_ProjectileType = _Type;
        }

        void SetSpeed(float _Speed)
        {
            m_AttackSpeed = _Speed;
        }

        void SetDir(Vector3 _Dir)
        {
            m_AttackDir = _Dir;
        }

        void SetDuration(float _Duration)
        {
            m_Duration = _Duration;
        }

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override;
        virtual void OnCollisionStay(Collider2D* _Other) override;
        virtual void OnCollisionExit(Collider2D* _Other) override;
    };
}
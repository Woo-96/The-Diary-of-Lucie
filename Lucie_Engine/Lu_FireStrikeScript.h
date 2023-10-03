#pragma once
#include "Lu_ProjectileScript.h"

namespace Lu
{
    class Animator;
    class FireStrikeScript : public ProjectileScript
    {
    public:
        FireStrikeScript();
        virtual ~FireStrikeScript();

    private:
        Animator* m_Animator;

    public:
        virtual void Initialize() override;
        virtual void Update() override {};

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override {};

    private:
        void CompleteAnimation();
    };
}


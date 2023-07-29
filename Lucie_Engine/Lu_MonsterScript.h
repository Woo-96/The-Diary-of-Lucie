#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
    class MonsterScript : public Script
    {
    public:
        struct tMonsterInfo
        {
            int HP;

            tMonsterInfo()
                : HP(100)
            {

            }
        };

    public:
        MonsterScript();
        virtual ~MonsterScript();

    private:
        tMonsterInfo m_MonsterInfo;

    public:
        tMonsterInfo& GetInfo()
        {
            return m_MonsterInfo;
        }

    public:
        virtual void Initialize() override {}
        virtual void Update() override {}

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override {}
        virtual void OnCollisionStay(Collider2D* _Other) override {}
        virtual void OnCollisionExit(Collider2D* _Other) override {}
    };
}


#pragma once
#include "Lu_ItemScript.h"

namespace Lu
{
    class WeaponScript : public ItemScript
    {
    public:
        enum class eWeaponType
        {
            Wand,
            Bow,
            Sword,
            None,
        };

    public:
        WeaponScript();
        virtual ~WeaponScript();

    private:
        eWeaponType m_WeaponType;

    public:
        void SetWeaponType(eWeaponType _Type)
        {
            m_WeaponType = _Type;
        }

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override;

    public:
        virtual bool ItemEfficacy() override;
    };
}

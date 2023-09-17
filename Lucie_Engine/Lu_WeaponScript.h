#pragma once
#include "Lu_ItemScript.h"
#include "Lu_ScriptEnums.h"

namespace Lu
{
    class WeaponScript : public ItemScript
    {
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
        eWeaponType GetWeaponType() const
        {
            return m_WeaponType;
        }

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override;

    public:
        virtual bool ItemEfficacy() override;
    };
}

#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class WeaponSlotScript;
	class QuickItemScript;
	class GameObject;
	class ItemScript;
	class InventoryScript : public UIScript
	{
	public:
		enum class eParts
		{
			Layout,
			Slot_1,
			Slot_2,
			Slot_3,
			Slot_4,
			Slot_5,
			Slot_6,
			Slot_7,
			Slot_8,
			Slot_9,
			Slot_10,
			EquipLabel,
			End,
		};

	public:
		InventoryScript();
		virtual ~InventoryScript();

	private:
		WeaponSlotScript*	m_WeaponSlot;
		QuickItemScript*	m_QuickSlot;

		GameObject*			m_arrParts[(int)eParts::End];
		bool				m_bActive;

		int					m_MaxCount;
		int					m_CurCount;

	public:
		void SetWeaponSlot(WeaponSlotScript* _WeaponSlot)
		{
			m_WeaponSlot = _WeaponSlot;
		}

		void SetQuickSlot(QuickItemScript* _QuickSlot)
		{
			m_QuickSlot = _QuickSlot;
		}

	public:
		WeaponSlotScript* GetWeaponSlot()	const
		{
			return m_WeaponSlot;
		}

		QuickItemScript* GetQuickSlot()	const
		{
			return m_QuickSlot;
		}

		bool GetActive()	const
		{
			return m_bActive;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	public:
		void InventoryOnOff();
		bool AddtoInventory(ItemScript* _Item);
		void UsetoInventory(int _SlotNumber);
		ItemScript* FindQuickItem();
		void EquipWeapon(int _SlotNumber, bool _bEquip);

	private:
		bool MouseCollision();
	};
}
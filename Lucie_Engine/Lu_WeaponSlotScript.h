#pragma once
#include "Lu_UIScript.h"
#include "Lu_Material.h"

namespace Lu
{
	class InventoryScript;
	class ItemScript;
	class WeaponSlotScript : public UIScript
	{
	public:
		enum class eSlotType
		{
			WeaponSlot_A,
			WeaponSlot_B,
			End,
		};

	public:
		WeaponSlotScript();
		virtual ~WeaponSlotScript();

	private:
		InventoryScript*			m_Inventory;
		eSlotType					m_CurType;
		GameObject*					m_arrIcon[(int)eSlotType::End];
		ItemScript*					m_arrItem[(int)eSlotType::End];

	public:
		void SetInventory(InventoryScript* _Inventory)
		{
			m_Inventory = _Inventory;
		}

	public:
		eSlotType GetCurSlotType()	const
		{
			return m_CurType;
		}

		ItemScript* GetSlotItem(eSlotType _Slot)	const
		{
			return m_arrItem[(int)_Slot];
		}

		bool IsCurSlotEmpty()
		{
			return m_arrItem[(int)m_CurType] == nullptr;
		}

	public:
		void ChangeSlot(eSlotType _Type);
		void ChangeSlot();	// 토글
		void EquipWeapon(ItemScript* _Item);	// 자동 장착
		void UnEquipWeapon(ItemScript* _Item);
		void WeaponChange(eSlotType _Slot, bool _bEquip);	// 수동 장착
		void ClearSlot(eSlotType _Slot);
	};
}
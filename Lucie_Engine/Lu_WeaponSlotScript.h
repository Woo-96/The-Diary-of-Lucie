#pragma once
#include "Lu_UIScript.h"
#include "Lu_Material.h"

namespace Lu
{
	class InventoryScript;
	class WeaponScript;
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
		WeaponScript*				m_arrWeapon[(int)eSlotType::End];

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

		WeaponScript* GetSlotItem(eSlotType _Slot)	const
		{
			return m_arrWeapon[(int)_Slot];
		}

		bool IsCurSlotEmpty()
		{
			return m_arrWeapon[(int)m_CurType] == nullptr;
		}

	public:
		void ChangeSlot(eSlotType _Type);
		void ChangeSlot();	// 토글
		void EquipWeapon(WeaponScript* _Item);	// 자동 장착
		void UnEquipWeapon(WeaponScript* _Item);
		void WeaponChange(eSlotType _Slot, bool _bEquip);	// 수동 장착
		void ClearSlot(eSlotType _Slot);
		void ResetWeaponSlot();

	public:
		virtual void SetActive(bool _b) override;
	};
}
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
		GameObject*					m_arrItem[(int)eSlotType::End];
		std::shared_ptr<Material>	m_arrMaterial[(int)eSlotType::End];
		eWeaponType					m_arrWeaponType[(int)eSlotType::End];

	public:
		void SetInventory(InventoryScript* _Inventory)
		{
			m_Inventory = _Inventory;
		}

	public:
		bool IsCurSlotEmpty()
		{
			return m_arrMaterial[(int)m_CurType] == nullptr;
		}

	public:
		virtual void Update() override;

	public:
		void ChangeSlot(eSlotType _Type);
		void ChangeSlot();	// ≈‰±€
		void EquipWeapon(ItemScript* _Item);
		void UnEquipWeapon(ItemScript* _Item);
	};
}
#pragma once
#include "Lu_UIScript.h"
#include "Lu_Material.h"

namespace Lu
{
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
		eSlotType					m_CurType;
		ItemScript*					m_arrItem[(int)eSlotType::End];
		Vector3						m_arrItemPos[(int)eSlotType::End];
		std::shared_ptr<Material>	m_arrMaterial[(int)eSlotType::End];

	public:
		virtual void Update() override;

	public:
		void ChangeSlot(eSlotType _Type);
		void ChangeSlot();	// ≈‰±€
		void EquipWeapon(ItemScript* _Item);
		void UnEquipWeapon(ItemScript* _Item);
	};
}
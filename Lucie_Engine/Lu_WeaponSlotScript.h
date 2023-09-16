#pragma once
#include "Lu_UIScript.h"

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
		eSlotType	m_CurType;
		ItemScript* m_CurItem;
		ItemScript* m_arrItem[(int)eSlotType::End];
		Vector3		m_arrItemPos[(int)eSlotType::End];

	public:
		virtual void Update() override;

	public:
		void ChangeSlot(eSlotType _Type);
		void ChangeSlot();	// ≈‰±€
		void SetWeaponSlotItem(ItemScript* _Item);
	};
}
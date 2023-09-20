#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class InventoryScript;
	class GameObject;
	class ItemScript;
	class QuickItemScript : public UIScript
	{
	public:
		QuickItemScript();
		virtual ~QuickItemScript();

	private:
		InventoryScript*	m_Inventory;
		GameObject*			m_CurIcon;
		ItemScript*			m_CurItem;

	public:
		void SetInventory(InventoryScript* _Inventory)
		{
			m_Inventory = _Inventory;
		}

	public:
		bool IsCurSlotEmpty()
		{
			return m_CurItem == nullptr;
		}

	public:
		void SetQuickSlotItem(ItemScript* _Item);
		void UseQuickSlotItem();
	};
}
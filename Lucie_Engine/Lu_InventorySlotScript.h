#pragma once
#include "Lu_ButtonScript.h"

namespace Lu
{
	class InventoryScript;
	class MeshRenderer;
	class ItemScript;
	class InventorySlotScript :	public ButtonScript
	{
	public:
		enum class eSlotState
		{
			Default,
			Equipped,
			End,
		};

	public:
		InventorySlotScript();
		virtual ~InventorySlotScript();

	private:
		InventoryScript*	m_Inventory;	
		eSlotState			m_SlotState;
		MeshRenderer*		m_MeshRender;
		ItemScript*			m_Item;

	public:
		void SetInventory(InventoryScript* _Inventory)
		{
			m_Inventory = _Inventory;
		}

		void SetSlotState(eSlotState _State);

		void SetMeshRender(MeshRenderer* _MeshRender)
		{
			m_MeshRender = _MeshRender;
		}

		void SetItem(ItemScript* _Item);

		void SetInventoryOnOFF(bool _b);

	public:
		bool IsSlotEmpty()
		{
			return m_Item == nullptr;
		}

	public:
		virtual void Update() override;

	private:
		void MaterialUpdate();
	};
}



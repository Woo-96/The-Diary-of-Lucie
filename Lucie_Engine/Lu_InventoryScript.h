#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
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
			End,
		};

	public:
		InventoryScript();
		virtual ~InventoryScript();

	private:
		GameObject*	m_arrParts[(int)eParts::End];
		bool		m_bActive;

		int			m_MaxCount;
		int			m_CurCount;

	public:
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
	};
}
#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class ItemScript;
	class QuickItemScript : public UIScript
	{
	public:
		QuickItemScript();
		virtual ~QuickItemScript();

	private:
		ItemScript* m_CurItem;

	public:
		void SetQuickSlotItem(ItemScript* _Item);
		void UseQuickSlotItem();
	};
}
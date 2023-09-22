#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class InventoryScript;
	class InventoryLayoutScript : public UIScript
	{
	public:
		InventoryLayoutScript();
		virtual ~InventoryLayoutScript();

	private:
		InventoryScript*	m_Inventory;
		std::wstring		m_Text;
		bool				m_bTextPrint;
		Vector2				m_TextPos;

	public:
		void SetInventory(InventoryScript* _Inven)
		{
			m_Inventory = _Inven;
		}

		void SetText(std::wstring _Text)
		{
			m_Text = _Text;
		}

		void SetTextPrint(bool _b)
		{
			m_bTextPrint = _b;
		}

	public:
		virtual void Render() override;

	private:
		void TextPrint();
		void PlayerInfoPrint();
	};
}

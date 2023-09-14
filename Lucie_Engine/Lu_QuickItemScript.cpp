#include "Lu_QuickItemScript.h"
#include "Lu_ItemScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

namespace Lu
{
	QuickItemScript::QuickItemScript()
	{
		SetName(L"QuickItemScript");
	}

	QuickItemScript::~QuickItemScript()
	{

	}

	void QuickItemScript::SetQuickSlotItem(ItemScript* _Item)
	{
		// 현재 퀵슬롯에 등록된 아이템이 없다면
		if (!m_CurItem)
		{
			// 아이템을 퀵슬롯에 등록 후 아이콘으로 변경
			m_CurItem = _Item;
			m_CurItem->SetItemState(ItemScript::eItemState::Icon);

			// 퀵슬롯 위치로 이동
			Transform* pTransform = m_CurItem->GetOwner()->GetComponent<Transform>();
			pTransform->SetPosition(GetOwner()->GetComponent<Transform>()->GetPosition());
		}
		else
		{
			// 이미 퀵 슬롯에 아이템이 있는데 바꾸고 싶은 경우..?
		}
	}

	void QuickItemScript::UseQuickSlotItem()
	{
		if (m_CurItem)
		{
			if (m_CurItem->ItemEfficacy())
				m_CurItem = nullptr;
		}
	}
}
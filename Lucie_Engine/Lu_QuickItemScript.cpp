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
		// ���� �����Կ� ��ϵ� �������� ���ٸ�
		if (!m_CurItem)
		{
			// �������� �����Կ� ��� �� ���������� ����
			m_CurItem = _Item;
			m_CurItem->SetItemState(ItemScript::eItemState::Icon);

			// ������ ��ġ�� �̵�
			Transform* pTransform = m_CurItem->GetOwner()->GetComponent<Transform>();
			pTransform->SetPosition(GetOwner()->GetComponent<Transform>()->GetPosition());
		}
		else
		{
			// �̹� �� ���Կ� �������� �ִµ� �ٲٰ� ���� ���..?
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
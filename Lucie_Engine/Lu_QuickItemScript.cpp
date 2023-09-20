#include "Lu_QuickItemScript.h"
#include "Lu_ItemScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_InventoryScript.h"

namespace Lu
{
	QuickItemScript::QuickItemScript()
		: m_Inventory(nullptr)
		, m_CurItem(nullptr)
	{
		SetName(L"QuickItemScript");

		m_CurIcon = object::Instantiate<GameObject>(Vector3(540.f, -345.f, 100.f), Vector3(48.f, 48.f, 100.f), eLayerType::UI);
		m_CurIcon->AddComponent<MeshRenderer>()->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		SceneManager::DontDestroyOnLoad(m_CurIcon);
	}

	QuickItemScript::~QuickItemScript()
	{

	}

	void QuickItemScript::SetQuickSlotItem(ItemScript* _Item)
	{
		if (!m_CurItem)
		{
			m_CurItem = _Item;
			m_CurIcon->GetComponent<MeshRenderer>()->SetMaterial(m_CurItem->GetOwner()->GetComponent<MeshRenderer>()->GetMaterial());
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
			{
				m_CurItem = nullptr;
				m_CurIcon->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
			}
		}
	}
}
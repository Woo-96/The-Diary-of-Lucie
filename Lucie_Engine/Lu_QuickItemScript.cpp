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

		m_CurIcon = object::Instantiate<GameObject>(Vector3(540.f, -345.f, 100.f), Vector3(48.f, 48.f, 140.f), eLayerType::UI);
		m_CurIcon->AddComponent<MeshRenderer>()->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		SceneManager::DontDestroyOnLoad(m_CurIcon);
	}

	QuickItemScript::~QuickItemScript()
	{

	}

	void QuickItemScript::ResetQuickSlotItem()
	{
		m_CurItem = nullptr;
		m_CurIcon->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
	}

	void QuickItemScript::SetQuickSlotItem(ItemScript* _Item)
	{
		m_CurItem = _Item;
		if (m_CurItem)
			m_CurIcon->GetComponent<MeshRenderer>()->SetMaterial(m_CurItem->GetOwner()->GetComponent<MeshRenderer>()->GetMaterial());
		else
			m_CurIcon->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
	}

	void QuickItemScript::UseQuickSlotItem()
	{
		if (m_CurItem)
		{
			if (m_CurItem->ItemEfficacy())
			{
				// 소비 아이템이 인벤토리에 존재한다면, 퀵 슬롯에 등록
				ItemScript* pItem = m_Inventory->FindQuickItem();
				if (pItem)
				{
					SetQuickSlotItem(pItem);
				}
				else
				{
					ResetQuickSlotItem();
				}
			}
		}
	}

	void QuickItemScript::SetActive(bool _b)
	{
		if (m_CurIcon)
			m_CurIcon->SetActive(_b);

		GetOwner()->SetActive(_b);
	}
}
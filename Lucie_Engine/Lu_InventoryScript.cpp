#include "Lu_InventoryScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_InventorySlotScript.h"
#include "Lu_ItemScript.h"

namespace Lu
{
	InventoryScript::InventoryScript()
		: m_arrParts{}
		, m_bActive(false)
		, m_MaxCount((int)eParts::Slot_10)
		, m_CurCount(0)
	{
		SetName(L"InventoryScript");
	}

	InventoryScript::~InventoryScript()
	{
	}

	void InventoryScript::Initialize()
	{
		// 원본 사이즈 1.5배
		{
			m_arrParts[(int)eParts::Layout] = object::Instantiate<GameObject>(Vector3(516.f, -80.f, 150.f), Vector3(360.f, 444.f, 100.f), eLayerType::UI);
			m_arrParts[(int)eParts::Layout]->SetName(L"Inventory_Layout");
			m_arrParts[(int)eParts::Layout]->SetActive(m_bActive);
			SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::Layout]);
			
			MeshRenderer* pMeshRender = m_arrParts[(int)eParts::Layout]->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"InvenBG_Mtrl"));
		}

		for (int i = (int)eParts::Slot_1; i <= (int)eParts::Slot_10; ++i)
		{
			Vector3 vStartPos;
			if (i <= (int)eParts::Slot_5)
			{
				vStartPos = Vector3(345.f, 40.f, 100.f);
				vStartPos.x += (i * 60);
			}
			else
			{
				int j = i - 5;
				vStartPos = Vector3(345.f, -20.f, 100.f);
				vStartPos.x += (j * 60);
			}

			m_arrParts[i] = object::Instantiate<GameObject>(vStartPos, Vector3(57.f, 57.f, 100.f), eLayerType::UI);
			m_arrParts[i]->SetName(L"Inventory_Slot");
			m_arrParts[i]->SetActive(m_bActive);
			SceneManager::DontDestroyOnLoad(m_arrParts[i]);

			MeshRenderer* pMeshRender = m_arrParts[i]->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Slot_Default_Mtrl"));

			InventorySlotScript* pSlotScript = m_arrParts[i]->AddComponent<InventorySlotScript>();
			pSlotScript->SetInventory(this);
			pSlotScript->SetMeshRender(pMeshRender);
		}
	}

	void InventoryScript::Update()
	{
	}

	void InventoryScript::Render()
	{
	}

	void InventoryScript::InventoryOnOff()
	{
		if (m_bActive)
			m_bActive = false;
		else
			m_bActive = true;

		for (int i = 0; i < (int)eParts::End; ++i)
		{
			if (m_arrParts[i])
				m_arrParts[i]->SetActive(m_bActive);

			if (i >= (int)eParts::Slot_1 && i <= (int)eParts::Slot_10)
			{
				m_arrParts[i]->GetComponent<InventorySlotScript>()->SetInventoryOnOFF(m_bActive);
			}
		}
	}

	bool InventoryScript::AddtoInventory(ItemScript* _Item)
	{
		// 인벤토리가 이미 꽉 차있다면 false
		if (m_MaxCount == m_CurCount)
			return false;

		++m_CurCount;

		InventorySlotScript* pSlotScript = m_arrParts[(int)m_CurCount]->GetComponent<InventorySlotScript>();
		
		if (!pSlotScript)
			return false;
		
		if (pSlotScript->IsSlotEmpty())
		{
			pSlotScript->SetItem(_Item);
			_Item->SetItemSlotNumber(m_CurCount);
		}
	}

	void InventoryScript::UsetoInventory(int _SlotNumber)
	{
		InventorySlotScript* pSlotScript = m_arrParts[_SlotNumber]->GetComponent<InventorySlotScript>();
		pSlotScript->SetItem(nullptr);
	}
}
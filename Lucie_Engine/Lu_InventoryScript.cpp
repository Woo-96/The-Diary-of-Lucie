#include "Lu_InventoryScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_InventorySlotScript.h"
#include "Lu_ItemScript.h"
#include "Lu_Camera.h"
#include "Lu_Input.h"
#include "Lu_PlayerScript.h"
#include "Lu_QuickItemScript.h"
#include "Lu_EquipLabelScript.h"
#include "Lu_WeaponSlotScript.h"
#include "Lu_InventoryLayoutScript.h"

namespace Lu
{
	InventoryScript::InventoryScript()
		: m_WeaponSlot(nullptr)
		, m_QuickSlot(nullptr)
		, m_arrParts{}
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

			m_arrParts[(int)eParts::Layout]->AddComponent<InventoryLayoutScript>()->SetInventory(this);
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
			pSlotScript->SetSlotNumber(i);
		}

		{
			m_arrParts[(int)eParts::EquipLabel] = object::Instantiate<GameObject>(Vector3(516.f, -80.f, 150.f), Vector3(28.5f, 28.5f, 100.f), eLayerType::UI);
			m_arrParts[(int)eParts::EquipLabel]->SetName(L"Inventory_EquipLabel");
			m_arrParts[(int)eParts::EquipLabel]->SetActive(m_bActive);
			SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::EquipLabel]);

			MeshRenderer* pMeshRender = m_arrParts[(int)eParts::EquipLabel]->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Equipped_Label_Mtrl"));

			m_arrParts[(int)eParts::EquipLabel]->AddComponent<EquipLabelScript>();
		}

		{
			m_arrParts[(int)eParts::GoldIcon] = object::Instantiate<GameObject>(Vector3(600.f, -217.f, 140.f), Vector3(31.5f, 33.f, 100.f), eLayerType::UI);
			m_arrParts[(int)eParts::GoldIcon]->SetName(L"Inventory_GoldIcon");
			m_arrParts[(int)eParts::GoldIcon]->SetActive(m_bActive);
			SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::GoldIcon]);

			MeshRenderer* pMeshRender = m_arrParts[(int)eParts::GoldIcon]->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"InvenGoldIcon_Mtrl"));
		}
	}

	void InventoryScript::Update()
	{
		// 인벤토리 위에 마우스가 존재한다면, 클릭 시 플레이어 공격 안나가도록 설정
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		if (MouseCollision())
			pPlayerScript->SetCantHit(true);

		// 무기 슬롯에 무기가 없다면, 라벨 비활성화
		if (m_WeaponSlot->IsCurSlotEmpty())
		{
			m_arrParts[(int)eParts::EquipLabel]->SetActive(false);
		}
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

			if (i == (int)eParts::EquipLabel && m_bActive)
			{
				if (m_WeaponSlot->IsCurSlotEmpty())
				{
					m_arrParts[(int)eParts::EquipLabel]->SetActive(false);
				}
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
			return true;
		}
		else
			return false;
	}

	void InventoryScript::UsetoInventory(int _SlotNumber)
	{
		for (int i = _SlotNumber; i <= (int)eParts::Slot_10; ++i)
		{
			// 마지막 슬롯일 경우, 그냥 비운다
			if (i == (int)eParts::Slot_10)
			{
				m_arrParts[i]->GetComponent<InventorySlotScript>()->SetItem(nullptr);
			}
			// 마지막 슬롯이 아닐 경우, 한 칸씩 당긴다
			else
			{
				ItemScript* pItem = m_arrParts[i + 1]->GetComponent<InventorySlotScript>()->GetItem();
				m_arrParts[i]->GetComponent<InventorySlotScript>()->SetItem(pItem);
				if(pItem != nullptr)
					pItem->SetItemSlotNumber(i);
			}
		}

		--m_CurCount;
	}

	ItemScript* InventoryScript::FindQuickItem()
	{
		ItemScript* pFindItem = nullptr;

		for (int i = (int)eParts::Slot_1; i <= (int)eParts::Slot_10; ++i)
		{
			InventorySlotScript* pSlotScript = m_arrParts[i]->GetComponent<InventorySlotScript>();
			ItemScript* pItem = pSlotScript->GetItem();
			if (pItem != nullptr && pItem->GetItemType() == ItemScript::eItemType::Jelly)
			{
				pFindItem = pItem;
				break;
			}
		}

		return pFindItem;
	}

	void InventoryScript::EquipWeapon(int _SlotNumber, bool _bEquip)
	{
		InventorySlotScript::eSlotState eState = InventorySlotScript::eSlotState::Default;
		if (_bEquip)
		{
			eState = InventorySlotScript::eSlotState::Equipped;
			Vector3 vLabelPos = m_arrParts[_SlotNumber]->GetComponent<Transform>()->GetPosition();
			vLabelPos.x -= 10.f;
			vLabelPos.y -= 10.f;
			vLabelPos.z = 90.f;
			m_arrParts[(int)eParts::EquipLabel]->GetComponent<Transform>()->SetPosition(vLabelPos);

			if (m_bActive)
				m_arrParts[(int)eParts::EquipLabel]->SetActive(_bEquip);
		}
		else
		{
			m_arrParts[(int)eParts::EquipLabel]->SetActive(_bEquip);
		}

		InventorySlotScript* pSlotScript = m_arrParts[_SlotNumber]->GetComponent<InventorySlotScript>();
		pSlotScript->SetSlotState(eState);
	}

	void InventoryScript::ChangeWeaponName(std::wstring _Name)
	{
		if (!_Name.empty())
		{
			InventoryLayoutScript* pLayout = m_arrParts[(int)eParts::Layout]->GetComponent<InventoryLayoutScript>();
			pLayout->SetText(_Name);
			pLayout->SetTextPrint(true);
		}
	}

	void InventoryScript::ResetInventory()
	{
		if(m_bActive)
			InventoryOnOff();
		m_CurCount = 0;

		for (int i = (int)eParts::Slot_1; i <= (int)eParts::Slot_10; ++i)
		{
			ItemScript* pItem = m_arrParts[i]->GetComponent<InventorySlotScript>()->GetItem();
			if (pItem != nullptr)
			{
				pItem->SetItemSlotNumber(-1);
				m_arrParts[i]->GetComponent<InventorySlotScript>()->SetItem(nullptr);
			}
		}
	}

	bool InventoryScript::MouseCollision()
	{
		Transform* pTransform = m_arrParts[(int)eParts::Layout]->GetComponent<Transform>();
		Vector3 vInvenWorldPos = pTransform->GetPosition();
		Vector2 vInvenScreenPos = SceneManager::GetActiveScene()->GetUICam()->WorldToScreen(vInvenWorldPos);
		Vector3 vInvenSize = pTransform->GetScale();

		Vector2 vMousePos = Input::GetMousePos();

		float fLeft = vInvenScreenPos.x - vInvenSize.x / 2.f;
		float fRight = vInvenScreenPos.x + vInvenSize.x / 2.f;
		float fTop = vInvenScreenPos.y + vInvenSize.y / 2.f;
		float fBottom = vInvenScreenPos.y - vInvenSize.y / 2.f;

		if (vMousePos.x < fLeft)
			return false;

		else if (vMousePos.y < fBottom)
			return false;

		else if (vMousePos.x > fRight)
			return false;

		else if (vMousePos.y > fTop)
			return false;

		return true;
	}
}
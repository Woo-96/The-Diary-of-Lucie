#include "Lu_InventorySlotScript.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_ItemScript.h"
#include "Lu_InventoryScript.h"
#include "Lu_QuickItemScript.h"
#include "Lu_WeaponSlotScript.h"

namespace Lu
{
	InventorySlotScript::InventorySlotScript()
		: m_Inventory(nullptr)
		, m_SlotState(eSlotState::Default)
		, m_MeshRender(nullptr)
		, m_Item(nullptr)
		, m_SlotNumber(-1)
	{
		SetName(L"InventorySlotScript");
	}

	InventorySlotScript::~InventorySlotScript()
	{
	}

	void InventorySlotScript::SetSlotState(eSlotState _State)
	{
		m_SlotState = _State;
		
		MaterialUpdate();
	}

	void InventorySlotScript::SetItem(ItemScript* _Item)
	{
		m_Item = _Item;
		if (m_Item)
		{
			m_Item->SetItemState(ItemScript::eItemState::Icon);

			// 아이콘의 위치를 조정
			Transform* pTransform = m_Item->GetOwner()->GetComponent<Transform>();
			pTransform->SetPosition(GetOwner()->GetComponent<Transform>()->GetPosition());

			// 인벤토리 활성화 여부에 따라 아이콘도 활성화/비활성화
			if (false == m_Inventory->GetActive())
				m_Item->GetOwner()->SetActive(false);
		}
	}

	void InventorySlotScript::SetInventoryOnOFF(bool _b)
	{
		if(m_Item)
			m_Item->GetOwner()->SetActive(_b);
	}

	void InventorySlotScript::Update()
	{
		ButtonScript::Update();

		// Default vs Hovered
		if (GetPrevBtnState() != GetCurBtnState())
			MaterialUpdate();
	}

	void InventorySlotScript::MaterialUpdate()
	{
		switch (GetCurBtnState())
		{
		case Lu::ButtonScript::eButtonState::Normal:
		{
			if (m_SlotState == eSlotState::Default)
				m_MeshRender->SetMaterial(Resources::Find<Material>(L"Slot_Default_Mtrl"));
			else
				m_MeshRender->SetMaterial(Resources::Find<Material>(L"Slot_Default_Equipped_Mtrl"));
		}
		break;
		case Lu::ButtonScript::eButtonState::MouseHovered:
		{
			if (m_SlotState == eSlotState::Default)
				m_MeshRender->SetMaterial(Resources::Find<Material>(L"Slot_Hovered_Mtrl"));
			else
				m_MeshRender->SetMaterial(Resources::Find<Material>(L"Slot_Hovered_Equipped_Mtrl"));
		}
		break;
		case Lu::ButtonScript::eButtonState::Click:
			break;
		case Lu::ButtonScript::eButtonState::Disable:
			break;
		case Lu::ButtonScript::eButtonState::End:
			break;
		default:
			break;
		}
	}

	void InventorySlotScript::ButtonHoveredEvent()
	{
		if(m_Item)
			m_Inventory->ChangeWeaponName(m_Item->GetItemName());
	}

	void InventorySlotScript::ButtonClickEvent()
	{
		if (m_Item)
		{
			switch (m_Item->GetItemType())
			{
			case ItemScript::eItemType::Weapon:
			{
				WeaponScript* pWeapon = (WeaponScript*)m_Item;

				WeaponSlotScript::eSlotType eCurSlot = m_Inventory->GetWeaponSlot()->GetCurSlotType();
				WeaponSlotScript::eSlotType eAnotherSlot = WeaponSlotScript::eSlotType::WeaponSlot_A;
				if (eCurSlot == WeaponSlotScript::eSlotType::WeaponSlot_A)
				{
					eAnotherSlot = WeaponSlotScript::eSlotType::WeaponSlot_B;
				}

				// 현재 슬롯에 무기가 장착되어 있는 경우
				if (!m_Inventory->GetWeaponSlot()->IsCurSlotEmpty())
				{
					// 그게 나임 : 장착 해제만 함
					if (pWeapon == m_Inventory->GetWeaponSlot()->GetSlotItem(eCurSlot))
						m_Inventory->GetWeaponSlot()->ClearSlot(eCurSlot);
					// 다른 애임 : 원래 있던 애를 장착 해제하고 나를 장착함
					else
					{
						m_Inventory->GetWeaponSlot()->ClearSlot(eCurSlot);
						m_Inventory->GetWeaponSlot()->EquipWeapon(pWeapon);
					}
				}
				// 내가 다른 슬롯에 등록되어 있는 경우
				else if (pWeapon == m_Inventory->GetWeaponSlot()->GetSlotItem(eAnotherSlot))
				{
					// 다른 슬롯의 나를 해제함
					m_Inventory->GetWeaponSlot()->ClearSlot(eAnotherSlot);
					// 현재 슬롯에 나를 장착함
					m_Inventory->GetWeaponSlot()->EquipWeapon(pWeapon);
				}
				// 그냥 빈 슬롯이었을 경우
				else
				{
					m_Inventory->GetWeaponSlot()->EquipWeapon(pWeapon);
				}
			}
				break;
			case ItemScript::eItemType::Jelly: 
			{
				// 현재 소모하려는 아이템이 퀵 슬롯에 등록된 아이템일 경우, 퀵 슬롯에서 제거
				if (m_Item == m_Inventory->GetQuickSlot()->GetCurItem())
				{
					m_Inventory->GetQuickSlot()->SetQuickSlotItem(nullptr);
				}

				// 아이템 소모 & 인벤토리에서 제거
				m_Item->ItemEfficacy();
			}
				break;
			default:
				break;
			}
		}
	}

	void InventorySlotScript::SetActive(bool _b)
	{
		if (m_Item)
			m_Item->GetOwner()->SetActive(_b);
	}
}
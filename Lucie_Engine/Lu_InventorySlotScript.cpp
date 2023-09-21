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

			// �������� ��ġ�� ����
			Transform* pTransform = m_Item->GetOwner()->GetComponent<Transform>();
			pTransform->SetPosition(GetOwner()->GetComponent<Transform>()->GetPosition());

			// �κ��丮 Ȱ��ȭ ���ο� ���� �����ܵ� Ȱ��ȭ/��Ȱ��ȭ
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

	void InventorySlotScript::ButtonClickEvent()
	{
		if (m_Item)
		{
			switch (m_Item->GetItemType())
			{
			case ItemScript::eItemType::Weapon:
			{
				WeaponSlotScript::eSlotType eCurSlot = m_Inventory->GetWeaponSlot()->GetCurSlotType();
				WeaponSlotScript::eSlotType eAnotherSlot = WeaponSlotScript::eSlotType::WeaponSlot_A;
				if (eCurSlot == WeaponSlotScript::eSlotType::WeaponSlot_A)
				{
					eAnotherSlot = WeaponSlotScript::eSlotType::WeaponSlot_B;
				}

				// �����Ϸ��� ���⸦ �̹� �����ϰ� �ִ� ���
				if (m_Item == m_Inventory->GetWeaponSlot()->GetSlotItem(eCurSlot))
					return;


				// ���� ���Կ� �������̴� ���Ⱑ �ִٸ� ���� ����
				if (!m_Inventory->GetWeaponSlot()->IsCurSlotEmpty())
				{
					m_Inventory->GetWeaponSlot()->ClearSlot(eCurSlot);
				}

				// ���� ����Ϸ��� ���Ⱑ �ٸ� ���Կ� ��ϵǾ� ���� ���
				if (m_Item == m_Inventory->GetWeaponSlot()->GetSlotItem(eAnotherSlot))
				{
					m_Inventory->GetWeaponSlot()->ClearSlot(eAnotherSlot);
				}

				// ���� ����
				m_Inventory->GetWeaponSlot()->EquipWeapon(m_Item);
			}
				break;
			case ItemScript::eItemType::Jelly: 
			{
				// ���� �Ҹ��Ϸ��� �������� �� ���Կ� ��ϵ� �������� ���, �� ���Կ��� ����
				if (m_Item == m_Inventory->GetQuickSlot()->GetCurItem())
				{
					m_Inventory->GetQuickSlot()->SetQuickSlotItem(nullptr);
				}

				// ������ �Ҹ� & �κ��丮���� ����
				m_Item->ItemEfficacy();
			}
				break;
			default:
				break;
			}
		}
	}
}
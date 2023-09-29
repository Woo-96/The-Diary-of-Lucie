#include "Lu_WeaponSlotScript.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_ItemScript.h"
#include "Lu_PlayerScript.h"
#include "Lu_WeaponScript.h"
#include "Lu_Object.h"
#include "Lu_InventoryScript.h"

namespace Lu
{
	WeaponSlotScript::WeaponSlotScript()
		: m_Inventory(nullptr)
		, m_CurType(eSlotType::WeaponSlot_A)
		, m_arrWeapon{}
	{
		for (int i = 0; i < (int)eSlotType::End; ++i)
		{
			Vector3 vPos = Vector3(632.f, -325.f, 100.f);

			if(i == (int)eSlotType::WeaponSlot_B)
				vPos = Vector3(647.f, -335.f, 100.f);

			m_arrIcon[i] = object::Instantiate<GameObject>(vPos, Vector3(48.f, 48.f, 100.f), eLayerType::UI);
			m_arrIcon[i]->AddComponent<MeshRenderer>()->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			SceneManager::DontDestroyOnLoad(m_arrIcon[i]);
		}
	}

	WeaponSlotScript::~WeaponSlotScript()
	{
	}

	void WeaponSlotScript::ChangeSlot(eSlotType _Type)
	{
		if (m_CurType == _Type)
			return;

		// 이전 슬롯(현재 슬롯)에 아이템이 등록되어 있었다면 가리기
		WeaponChange(m_CurType, false);

		// 슬롯 교체
		m_CurType = _Type;

		// 슬롯의 재질 교체
		if (m_CurType == eSlotType::WeaponSlot_A)
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotA_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotA.png"));
		}
		else
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotB_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotB.png"));
		}

		// 현재 슬롯에 아이템 등록
		WeaponChange(m_CurType, true);

		// 슬롯 교체 SFX
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EquipChangeSFX", L"..\\Resources\\Sound\\SFX\\Player\\EquipChangeSFX.ogg"));
		pSFX->Play();
	}

	void WeaponSlotScript::ChangeSlot()
	{
		if (m_CurType == eSlotType::WeaponSlot_A)
		{
			ChangeSlot(eSlotType::WeaponSlot_B);
		}
		else
		{
			ChangeSlot(eSlotType::WeaponSlot_A);
		}
	}

	void WeaponSlotScript::EquipWeapon(WeaponScript* _Item)
	{
		// 장착한 아이템 저장
		m_arrWeapon[(int)m_CurType] = _Item;

		// 아이콘을 장착한 아이템으로 변경
		m_arrIcon[(int)m_CurType]->GetComponent<MeshRenderer>()->SetMaterial(m_arrWeapon[(int)m_CurType]->GetOwner()->GetComponent<MeshRenderer>()->GetMaterial());

		// 장착한 아이템의 인벤토리 슬롯의 상태 동기화
		m_Inventory->EquipWeapon(_Item->GetItemSlotNumber(), true);

		// 장착한 아이템의 효과를 플레이어에게 적용
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		if (pPlayerScript)
			pPlayerScript->SetWeaponType(m_arrWeapon[(int)m_CurType]->GetWeaponType(), m_arrWeapon[(int)m_CurType]->GetItemOption());
	}

	void WeaponSlotScript::UnEquipWeapon(WeaponScript* _Item)
	{
	}

	void WeaponSlotScript::WeaponChange(eSlotType _Slot, bool _bEquip)
	{
		if (_bEquip)
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

			if (m_arrWeapon[(int)_Slot])
			{
				m_arrIcon[(int)_Slot]->GetComponent<MeshRenderer>()->SetMaterial(m_arrWeapon[(int)_Slot]->GetOwner()->GetComponent<MeshRenderer>()->GetMaterial());

				m_Inventory->EquipWeapon(m_arrWeapon[(int)_Slot]->GetItemSlotNumber(), true);

				WeaponScript* pItemScript = (WeaponScript*)m_arrWeapon[(int)_Slot];
				if (pPlayerScript)
				{
					pPlayerScript->SetWeaponType(pItemScript->GetWeaponType(), pItemScript->GetItemOption());
				}
			}
			else
			{
				if (pPlayerScript)
				{
					pPlayerScript->SetWeaponType(eWeaponType::None, 0);
				}
			}

		}
		else
		{
			m_arrIcon[(int)_Slot]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);

			if (m_arrWeapon[(int)_Slot])
			{
				m_Inventory->EquipWeapon(m_arrWeapon[(int)_Slot]->GetItemSlotNumber(), false);

				GameObject* pPlayer = SceneManager::FindPlayer();
				PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
				pPlayerScript->SetWeaponType(m_arrWeapon[(int)_Slot]->GetWeaponType(), -m_arrWeapon[(int)_Slot]->GetItemOption());
				pPlayerScript->SetWeaponType(eWeaponType::None, 0);
			}
		}
	}

	void WeaponSlotScript::ClearSlot(eSlotType _Slot)
	{
		if (_Slot == m_CurType)
		{
			m_arrIcon[(int)_Slot]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);

			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			pPlayerScript->SetWeaponType(m_arrWeapon[(int)_Slot]->GetWeaponType(), -m_arrWeapon[(int)_Slot]->GetItemOption());
			pPlayerScript->SetWeaponType(eWeaponType::None, 0);
		}

		if (m_arrWeapon[(int)_Slot])
			m_Inventory->EquipWeapon(m_arrWeapon[(int)_Slot]->GetItemSlotNumber(), false);

		m_arrWeapon[(int)_Slot] = nullptr;
	}

	void WeaponSlotScript::ResetWeaponSlot()
	{
		m_CurType = eSlotType::WeaponSlot_A;
		m_arrIcon[(int)eSlotType::WeaponSlot_A]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		m_arrIcon[(int)eSlotType::WeaponSlot_B]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		m_arrWeapon[(int)eSlotType::WeaponSlot_A] = nullptr;
		m_arrWeapon[(int)eSlotType::WeaponSlot_B] = nullptr;
	}
}
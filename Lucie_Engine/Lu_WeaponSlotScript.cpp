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

		// ���� ����(���� ����)�� �������� ��ϵǾ� �־��ٸ� ������
		WeaponChange(m_CurType, false);

		// ���� ��ü
		m_CurType = _Type;

		// ������ ���� ��ü
		if (m_CurType == eSlotType::WeaponSlot_A)
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotA_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotA.png"));
		}
		else
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotB_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotB.png"));
		}

		// ���� ���Կ� ������ ���
		WeaponChange(m_CurType, true);

		// ���� ��ü SFX
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
		// ������ ������ ����
		m_arrWeapon[(int)m_CurType] = _Item;

		// �������� ������ ���������� ����
		m_arrIcon[(int)m_CurType]->GetComponent<MeshRenderer>()->SetMaterial(m_arrWeapon[(int)m_CurType]->GetOwner()->GetComponent<MeshRenderer>()->GetMaterial());

		// ������ �������� �κ��丮 ������ ���� ����ȭ
		m_Inventory->EquipWeapon(_Item->GetItemSlotNumber(), true);

		// ������ �������� ȿ���� �÷��̾�� ����
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
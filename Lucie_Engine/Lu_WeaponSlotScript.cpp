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
		, m_arrItem{}
		, m_arrMaterial{}
	{
		for (int i = 0; i < (int)eSlotType::End; ++i)
		{
			Vector3 vPos = Vector3(632.f, -325.f, 100.f);

			if(i == (int)eSlotType::WeaponSlot_B)
				vPos = Vector3(647.f, -335.f, 100.f);

			m_arrItem[i] = object::Instantiate<GameObject>(vPos, Vector3(48.f, 48.f, 100.f), eLayerType::UI);
			m_arrItem[i]->AddComponent<MeshRenderer>()->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			m_arrWeaponType[i] = eWeaponType::None;
			SceneManager::DontDestroyOnLoad(m_arrItem[i]);
		}
	}

	WeaponSlotScript::~WeaponSlotScript()
	{
	}

	void WeaponSlotScript::Update()
	{
	}

	void WeaponSlotScript::ChangeSlot(eSlotType _Type)
	{
		if (m_CurType == _Type)
			return;

		// 이전 슬롯(현재 슬롯)에 아이템이 등록되어 있었다면 가리기
		if (m_arrMaterial[(int)m_CurType])
		{
			m_arrItem[(int)m_CurType]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		}

		// 슬롯 교체
		m_CurType = _Type;
		m_arrItem[(int)m_CurType]->GetComponent<MeshRenderer>()->SetMaterial(m_arrMaterial[(int)m_CurType]);

		// 슬롯의 재질 교체
		if (m_CurType == eSlotType::WeaponSlot_A)
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotA_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotA.png"));
		}
		else
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotB_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotB.png"));
		}

		// 슬롯의 아이템 렌더링 및 플레이어에 적용
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		if (pPlayerScript)
		{
			if(m_arrWeaponType[(int)m_CurType] == eWeaponType::None)
				pPlayerScript->SetWeaponType(eWeaponType::Wand);
			else
				pPlayerScript->SetWeaponType(m_arrWeaponType[(int)m_CurType]);
		}

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

	void WeaponSlotScript::EquipWeapon(ItemScript* _Item)
	{
		WeaponScript* pItemScript = (WeaponScript*)_Item;
		m_arrWeaponType[(int)m_CurType] = pItemScript->GetWeaponType();
		m_arrMaterial[(int)m_CurType] = _Item->GetOwner()->GetComponent<MeshRenderer>()->GetMaterial();
		m_arrItem[(int)m_CurType]->GetComponent<MeshRenderer>()->SetMaterial(m_arrMaterial[(int)m_CurType]);

		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		if (pPlayerScript)
		{
			pPlayerScript->SetWeaponType(m_arrWeaponType[(int)m_CurType]);
		}
	}

	void WeaponSlotScript::UnEquipWeapon(ItemScript* _Item)
	{
	}
}
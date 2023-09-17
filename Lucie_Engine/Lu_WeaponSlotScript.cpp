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

namespace Lu
{
	WeaponSlotScript::WeaponSlotScript()
		: m_CurType(eSlotType::WeaponSlot_A)
		, m_arrItem{}
		, m_arrMaterial{}
	{
		m_arrItemPos[(int)eSlotType::WeaponSlot_A] = Vector3(632.f, -325.f, 100.f);
		m_arrItemPos[(int)eSlotType::WeaponSlot_B] = Vector3(647.f, -335.f, 100.f);
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
		if (m_arrItem[(int)m_CurType])
		{
			m_arrItem[(int)m_CurType]->GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		}

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


		// 슬롯의 아이템 렌더링 및 플레이어에 적용
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		if (m_arrItem[(int)m_CurType])
		{
			m_arrItem[(int)m_CurType]->GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(m_arrMaterial[(int)m_CurType]);
			if (pPlayerScript)
			{
				WeaponScript* pItemScript = (WeaponScript*)m_arrItem[(int)m_CurType];
				if(pItemScript)
					pPlayerScript->SetWeaponType(pItemScript->GetWeaponType());
			}
		}
		else
		{
			pPlayerScript->SetWeaponType(eWeaponType::Wand);
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
		if (!m_arrItem[(int)m_CurType])
		{
			m_arrItem[(int)m_CurType] = _Item;
			m_arrItem[(int)m_CurType]->SetItemState(ItemScript::eItemState::Icon);

			Transform* pTransform = m_arrItem[(int)m_CurType]->GetOwner()->GetComponent<Transform>();
			pTransform->SetPosition(m_arrItemPos[(int)m_CurType]);

			m_arrMaterial[(int)m_CurType] = m_arrItem[(int)m_CurType]->GetOwner()->GetComponent<MeshRenderer>()->GetMaterial();

			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			if (pPlayerScript)
			{
				WeaponScript* pItemScript = (WeaponScript*)m_arrItem[(int)m_CurType];
				pPlayerScript->SetWeaponType(pItemScript->GetWeaponType());
			}
		}
		else
		{
			// 웨폰 슬롯에 아이템을 교체하고 싶은 경우
		}
	}

	void WeaponSlotScript::UnEquipWeapon(ItemScript* _Item)
	{
	}
}
#include "Lu_WeaponSlotScript.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_ItemScript.h"

namespace Lu
{
	WeaponSlotScript::WeaponSlotScript()
		: m_CurType(eSlotType::WeaponSlot_A)
		, m_CurItem(nullptr)
		, m_arrItem{}
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
		m_CurType = _Type;

		if (m_CurType == eSlotType::WeaponSlot_A)
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotA_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotA.png"));
		}
		else
		{
			GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(Resources::Load<Texture>(L"weaponSlotB_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotB.png"));
		}

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

	void WeaponSlotScript::SetWeaponSlotItem(ItemScript* _Item)
	{
		if (!m_arrItem[(int)m_CurType])
		{
			m_arrItem[(int)m_CurType] = _Item;
			m_arrItem[(int)m_CurType]->SetItemState(ItemScript::eItemState::Icon);

			Transform* pTransform = m_arrItem[(int)m_CurType]->GetOwner()->GetComponent<Transform>();
			pTransform->SetPosition(m_arrItemPos[(int)m_CurType]);
		}
		else
		{
			// 웨폰 슬롯에 아이템을 교체하고 싶은 경우
		}
	}
}
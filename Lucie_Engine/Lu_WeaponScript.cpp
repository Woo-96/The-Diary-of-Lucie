#include "Lu_WeaponScript.h"
#include "Lu_PlayerScript.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_SceneManager.h"
#include "Lu_WeaponChoiceScene.h"
#include "Lu_InventoryScript.h"
#include "Lu_WeaponSlotScript.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"

namespace Lu
{
	WeaponScript::WeaponScript()
		: m_WeaponType(eWeaponType::None)
	{
		SetName(L"WeaponScript");
		SetItemType(eItemType::Weapon);
	}

	WeaponScript::~WeaponScript()
	{
	}

	void WeaponScript::Update()
	{
		ItemScript::Update();

		std::shared_ptr<Material> pMaterial = GetOwner()->GetComponent<MeshRenderer>()->GetMaterial();
		if (pMaterial)
		{
			int bPointSamplerUse = true;
			pMaterial->SetScalarParam(Lu::graphics::SCALAR_PARAM::INT_2, &bPointSamplerUse);
		}
	}

	void WeaponScript::OnCollisionEnter(Collider2D* _Other)
	{
		GameObject* pPlayer = _Other->GetOwner();
		if (pPlayer->GetLayerIndex() == (int)eLayerType::Player)
		{
			ItemEfficacy();
		}
	}

	bool WeaponScript::ItemEfficacy()
	{
		// 무기 선택 튜토리얼 예외처리
		WeaponChoiceScene* pCurScene = dynamic_cast<WeaponChoiceScene*>(SceneManager::GetActiveScene());
		if (pCurScene)
		{
			pCurScene->PlayerWeaponGet(m_WeaponType);
		}

		// 인벤토리에 아이템 추가
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		if (!pPlayerScript)
			return false;

		InventoryScript* pInven = (InventoryScript*)pPlayerScript->GetUI(PlayerScript::eUI::Inventory);
		if (!pInven)
			return false;

		if (pInven->AddtoInventory(this))
		{
			// 웨폰 퀵 슬롯에 아이템 추가
			WeaponSlotScript* pSlot = (WeaponSlotScript*)pPlayerScript->GetUI(PlayerScript::eUI::WeaponSlot);
			if (pSlot)
			{
				if (pSlot->IsCurSlotEmpty())
					pSlot->EquipWeapon(this);
			}
		
			// 아이템 획득 효과음
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
			pSFX->SetClip(Resources::Load<AudioClip>(L"GetItemSFX", L"..\\Resources\\Sound\\SFX\\Player\\GetItemSFX.ogg"));
			pSFX->Play();
		}

		return true;
	}
}
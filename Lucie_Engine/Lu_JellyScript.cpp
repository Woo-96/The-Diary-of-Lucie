#include "Lu_JellyScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_PlayerScript.h"
#include "Lu_QuickItemScript.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_InventoryScript.h"

namespace Lu
{
	JellyScript::JellyScript()
	{
		SetName(L"JellyScript");
		SetItemType(eItemType::Jelly);
	}

	JellyScript::~JellyScript()
	{

	}

	void JellyScript::Update()
	{
		ItemScript::Update();
	}

	void JellyScript::OnCollisionEnter(Collider2D* _Other)
	{
		GameObject* pPlayer = _Other->GetOwner();
		if (pPlayer->GetLayerIndex() == (int)eLayerType::Player)
		{
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			if (pPlayerScript)
			{
				InventoryScript* pInven = (InventoryScript*)pPlayerScript->GetUI(PlayerScript::eUI::Inventory);
				if (pInven->AddtoInventory(this))
				{
					QuickItemScript* pQuickItem = (QuickItemScript*)pPlayerScript->GetUI(PlayerScript::eUI::QuickItem);
					if (pQuickItem)
					{
						if (pQuickItem->IsCurSlotEmpty())
							pQuickItem->SetQuickSlotItem(this);
					}

					AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
					pSFX->SetClip(Resources::Load<AudioClip>(L"GetItemSFX", L"..\\Resources\\Sound\\SFX\\Player\\GetItemSFX.ogg"));
					pSFX->Play();
				}
			}
		}
	}

	bool JellyScript::ItemEfficacy()
	{
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

		int iMaxHP = pPlayerScript->GetPlayerInfo().MaxHP;
		int iCurHP = pPlayerScript->GetPlayerInfo().CurHP;

		if (iCurHP + 2 > iMaxHP)
			return false;

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"RecoverySFX", L"..\\Resources\\Sound\\SFX\\Player\\RecoverySFX.ogg"));
		pSFX->Play();

		pPlayerScript->InflictDamage(-2);

		// 인벤토리에서 제거
		InventoryScript* pInventory = (InventoryScript*)pPlayerScript->GetUI(PlayerScript::eUI::Inventory);
		pInventory->UsetoInventory(GetItemSlotNumber());

		SceneManager::RemoveFromDontDestroyOnLoad(GetOwner());
		object::Destroy(GetOwner());

		return true;
	}
}
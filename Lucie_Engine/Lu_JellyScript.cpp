#include "Lu_JellyScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_PlayerScript.h"
#include "Lu_QuickItemScript.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"

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
			// 원래 인벤토리에 추가하고, 인벤토리에서 퀵슬롯이 비어있다면 등록해줘야함!
			// 따로 퀵슬롯으로 등록할 수 있었는지는 기억이 안남ㅋ (소모하는 아이템이면 자동으로 퀵 슬롯 등록했나?)
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			if (pPlayerScript)
			{
				QuickItemScript* pQuickItem = (QuickItemScript*)pPlayerScript->GetUI(PlayerScript::eUI::QuickItem);
				pQuickItem->SetQuickSlotItem(this);

				AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
				pSFX->SetClip(Resources::Load<AudioClip>(L"GetItemSFX", L"..\\Resources\\Sound\\SFX\\Player\\GetItemSFX.ogg"));
				pSFX->Play();
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
		SceneManager::RemoveFromDontDestroyOnLoad(GetOwner());
		object::Destroy(GetOwner());

		return true;
	}
}
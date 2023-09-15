#include "Lu_CoinScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"
#include "Lu_NumberScript.h"
#include "Lu_SceneManager.h"
#include "Lu_Object.h"
#include "Lu_Resources.h"
#include "Lu_Animator.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	CoinScript::CoinScript()
		: m_CoinType(eCoinType::None)
		, m_Animator(nullptr)
	{
		SetName(L"CoinScript");
		SetItemType(eItemType::Coin);
		SetItemState(eItemState::Drop);
	}

	CoinScript::~CoinScript()
	{

	}

	void CoinScript::Update()
	{
		ItemScript::Update();
	}

	void CoinScript::OnCollisionEnter(Collider2D* _Other)
	{
		GameObject* pPlayer = _Other->GetOwner();
		if (pPlayer->GetLayerIndex() == (int)eLayerType::Player)
		{
			ItemEfficacy();
		}
	}

	bool CoinScript::ItemEfficacy()
	{
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

		if(!pPlayerScript)
			return false;

		int iGold = 0;
		switch (m_CoinType)
		{
		case Lu::CoinScript::eCoinType::Copper:
			iGold = 1;
			break;
		case Lu::CoinScript::eCoinType::Silver:
			iGold = 3;
			break;
		case Lu::CoinScript::eCoinType::Gold:
			iGold = 5;
			break;
		}

		if (iGold <= 0 || iGold > 5)
			return false;

		int iCurGold = pPlayerScript->GetPlayerInfo().CurGold;
		iCurGold += iGold;
		if (iCurGold > pPlayerScript->GetPlayerInfo().MaxGold)
			return false;
		else
		{
			pPlayerScript->GetCoin(iGold);

			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
			pSFX->SetClip(Resources::Load<AudioClip>(L"CoinSFX", L"..\\Resources\\Sound\\SFX\\Player\\CoinSFX.ogg"));
			pSFX->Play();

			object::Destroy(GetOwner());

			return true;
		}
	}

	void CoinScript::CreateCoinAnimation()
	{
		std::shared_ptr<Texture> pAtlas;
		std::wstring wstrAnimName;
			
		switch (m_CoinType)
		{
		case Lu::CoinScript::eCoinType::None:
			break;
		case Lu::CoinScript::eCoinType::Copper:
		{
			pAtlas = Resources::Load<Texture>(L"CopperCoin_Tex", L"..\\Resources\\Texture\\Item\\CopperCoin.png");
			wstrAnimName = L"Copper_Coin";

		}
			break;
		case Lu::CoinScript::eCoinType::Silver:
		{
			pAtlas = Resources::Load<Texture>(L"SilverCoin_Tex", L"..\\Resources\\Texture\\Item\\SilverCoin.png");
			wstrAnimName = L"Silver_Coin";
		}
			break;
		case Lu::CoinScript::eCoinType::Gold:
		{
			pAtlas = Resources::Load<Texture>(L"GoldCoin_Tex", L"..\\Resources\\Texture\\Item\\GoldCoin.png");
			wstrAnimName = L"Gold_Coin";
		}
			break;
		default:
			break;
		}

		m_Animator->Create(wstrAnimName, pAtlas, Vector2(0.f, 0.f), Vector2(16.f, 16.f), 8, Vector2(16.f, 16.f));
		m_Animator->PlayAnimation(wstrAnimName, true);
	}
}
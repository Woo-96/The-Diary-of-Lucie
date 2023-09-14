#include "Lu_JellyScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_PlayerScript.h"
#include "Lu_QuickItemScript.h"

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
			// ���� �κ��丮�� �߰��ϰ�, �κ��丮���� �������� ����ִٸ� ����������!
			// ���� ���������� ����� �� �־������� ����� �ȳ��� (�Ҹ��ϴ� �������̸� �ڵ����� �� ���� ����߳�?)
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			if (pPlayerScript)
			{
				QuickItemScript* pQuickItem = (QuickItemScript*)pPlayerScript->GetUI(PlayerScript::eUI::QuickItem);
				pQuickItem->SetQuickSlotItem(this);
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

		pPlayerScript->InflictDamage(-2);
		SceneManager::RemoveFromDontDestroyOnLoad(GetOwner());
		object::Destroy(GetOwner());

		return true;
	}
}
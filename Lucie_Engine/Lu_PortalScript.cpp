#include "Lu_PortalScript.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	PortalScript::PortalScript()
		: m_SpawnPos(Vector2::Zero)
	{
		SetName(L"PortalScript");
	}

	PortalScript::~PortalScript()
	{

	}

	void PortalScript::Initialize()
	{
		if (m_CurSceneName.empty() || m_NextSceneName.empty())
			return;

		//L"WeaponChoiceScene"
		//L"DiceScene"
		//L"StoreScene"
		//L"Nomal1Scene"
		//L"Nomal2Scene";
		//L"MidBossScene";
		//L"NextFloorScene"
		//L"BossWayScene"
		//L"BossScene"

		if (L"WeaponChoiceScene" == m_CurSceneName)
		{
			if (L"Nomal1Scene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(0.f, -600.f);
			}
		}
		else if (L"Nomal1Scene" == m_CurSceneName)
		{
			if (L"WeaponChoiceScene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(0.f, 450.f);
			}
			else if (L"DiceScene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(800.f, -270.f);
			}
		}
		else if (L"DiceScene" == m_CurSceneName)
		{
			if (L"Nomal1Scene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(-700.f, 0.f);
			}
			else if (L"Nomal2Scene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(0.f, -500.f);
			}
		}
		else if (L"Nomal2Scene" == m_CurSceneName)
		{
			if (L"DiceScene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(0.f, 550.f);
			}
			else if (L"MidBossScene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(-750.f, -100.f);
			}
			//else if (L"StoreScene" == m_NextSceneName)
			//{
			//	m_SpawnPos = Vector2(0.f, -500.f);
			//}
		}
		//else if (L"StoreScene" == m_CurSceneName)
		//{
		//	if (L"Nomal2Scene" == m_NextSceneName)
		//	{
		//		m_SpawnPos = Vector2(0.f, 550.f);
		//	}
		//	else if (L"MidBossScene" == m_NextSceneName)
		//	{
		//		m_SpawnPos = Vector2(-800.f, -100.f);
		//	}
		//}
		else if (L"MidBossScene" == m_CurSceneName)
		{
			if (L"Nomal2Scene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(700.f, -30.f);
			}
			//if (L"StoreScene" == m_NextSceneName)
			//{
			//	m_SpawnPos = Vector2(0.f, -600.f);
			//}
			else if (L"NextFloorScene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(0.f, 450.f);
			}
		}

		else if (L"NextFloorScene" == m_CurSceneName)
		{
			if (L"MidBossScene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(0.f, -750.f);
			}
		}

		else if (L"BossWayScene" == m_CurSceneName)
		{
			if (L"BossScene" == m_NextSceneName)
			{
				m_SpawnPos = Vector2(-45.f, -800.f);
			}
		}
	}

	void PortalScript::OnCollisionStay(Collider2D* _Other)
	{
		int iLayer = _Other->GetOwner()->GetLayerIndex();
		if ((int)eLayerType::Player == iLayer)
		{
			SceneManager::LoadScene(m_NextSceneName);

			GameObject* pPlayer = SceneManager::FindPlayer();
			pPlayer->GetComponent<Transform>()->SetPosition(Vector3(m_SpawnPos.x, m_SpawnPos.y, 500.f));
		}
	}
}
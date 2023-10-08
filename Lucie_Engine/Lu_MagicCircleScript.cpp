#include "Lu_MagicCircleScript.h"
#include "Lu_Animator.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	MagicCircleScript::MagicCircleScript()
		: m_Animator(nullptr)
	{
		SetName(L"MagicCircleScript");
	}

	MagicCircleScript::~MagicCircleScript()
	{
	}

	void MagicCircleScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		CreateMagicCircleAnimation();

		m_Animator->PlayAnimation(L"MagicCircle_1", true);
	}

	void MagicCircleScript::OnCollisionStay(Collider2D* _Other)
	{
		int iLayer = _Other->GetOwner()->GetLayerIndex();
		if ((int)eLayerType::Player == iLayer)
		{
			SceneManager::LoadScene(L"BossWayScene");

			GameObject* pPlayer = SceneManager::FindPlayer();
			pPlayer->GetComponent<Transform>()->SetPosition(Vector3(0.f, -750.f, 500.f));

			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
			pSFX->SetClip(Resources::Load<AudioClip>(L"floorStart", L"..\\Resources\\Sound\\SFX\\floorStart.ogg"));
			pSFX->Play();
		}
	}

	void MagicCircleScript::CreateMagicCircleAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"NextFloorPortal_Tex", L"..\\Resources\\Texture\\Map\\Stage\\NextFloorPortal.png");

		m_Animator->Create(L"MagicCircle_1", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 5, Vector2(192.f, 192.f));
		m_Animator->CompleteEvent(L"MagicCircle_1") = std::bind(&MagicCircleScript::MagicCircle1Finished, this);

		m_Animator->Create(L"MagicCircle_2", pAtlas, Vector2(0.f, 192.f), Vector2(192.f, 192.f), 5, Vector2(192.f, 192.f));
		m_Animator->CompleteEvent(L"MagicCircle_2") = std::bind(&MagicCircleScript::MagicCircle2Finished, this);

		m_Animator->Create(L"MagicCircle_3", pAtlas, Vector2(0.f, 384.f), Vector2(192.f, 192.f), 5, Vector2(192.f, 192.f));
		m_Animator->CompleteEvent(L"MagicCircle_3") = std::bind(&MagicCircleScript::MagicCircle3Finished, this);

		m_Animator->Create(L"MagicCircle_4", pAtlas, Vector2(0.f, 576.f), Vector2(192.f, 192.f), 5, Vector2(192.f, 192.f));
		m_Animator->CompleteEvent(L"MagicCircle_4") = std::bind(&MagicCircleScript::MagicCircle4Finished, this);

		m_Animator->Create(L"MagicCircle_5", pAtlas, Vector2(0.f, 768.f), Vector2(192.f, 192.f), 5, Vector2(192.f, 192.f));
		m_Animator->CompleteEvent(L"MagicCircle_5") = std::bind(&MagicCircleScript::MagicCircle5Finished, this);
	}

	void MagicCircleScript::MagicCircle1Finished()
	{
		m_Animator->PlayAnimation(L"MagicCircle_2", true);
	}

	void MagicCircleScript::MagicCircle2Finished()
	{
		m_Animator->PlayAnimation(L"MagicCircle_3", true);
	}

	void MagicCircleScript::MagicCircle3Finished()
	{
		m_Animator->PlayAnimation(L"MagicCircle_4", true);
	}

	void MagicCircleScript::MagicCircle4Finished()
	{
		m_Animator->PlayAnimation(L"MagicCircle_5", true);
	}

	void MagicCircleScript::MagicCircle5Finished()
	{
		m_Animator->PlayAnimation(L"MagicCircle_1", true);
	}
}
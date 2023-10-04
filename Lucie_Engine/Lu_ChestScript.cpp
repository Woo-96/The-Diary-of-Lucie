#include "Lu_ChestScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animation.h"
#include "Lu_Resources.h"
#include "Lu_Time.h"
#include "Lu_Input.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_WeaponScript.h"

namespace Lu
{
	ChestScript::ChestScript()
		: m_Animator(nullptr)
		, m_ChestFX(nullptr)
		, m_Time(0.f)
		, m_bMove(true)
		, m_bOpen(false)
	{
	}

	ChestScript::~ChestScript()
	{
	}

	void ChestScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		if (m_Animator)
		{
			std::shared_ptr<Texture> pAtlas
				= Resources::Load<Texture>(L"Chest_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Chest.png");

			m_Animator->Create(L"Chest_Close", pAtlas, Vector2::Zero, Vector2(96.f, 96.f), 1, Vector2(96.f, 96.f));
			m_Animator->Create(L"Chest_Open", pAtlas, Vector2::Zero, Vector2(96.f, 96.f), 4, Vector2(96.f, 96.f));
			m_Animator->CompleteEvent(L"Chest_Open") = std::bind(&ChestScript::CreateReward, this);
			m_Animator->Create(L"Chest_Open_Stop", pAtlas, Vector2(288.f, 0.f), Vector2(96.f, 96.f), 1, Vector2(96.f, 96.f));

			m_Animator->PlayAnimation(L"Chest_Close", true);
		}
	}

	void ChestScript::Update()
	{
		if (m_bMove)
		{
			m_Time += (float)Time::DeltaTime();

			Transform* pTransform = GetOwner()->GetComponent<Transform>();
			Vector3 vPos = pTransform->GetPosition();
			vPos.y += (-1.f) * (200.f) * (float)Time::DeltaTime();
			pTransform->SetPosition(vPos);

			if (m_Time >= 0.2f)
			{
				m_bMove = false;

				// 상자 도착 이펙트 생성
				m_ChestFX = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(288.f, 288.f, 200.f), eLayerType::FX);
				m_ChestFX->SetName(L"ChestOpenFX2");
				MeshRenderer* pMeshRender = m_ChestFX->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"ChestOpenFX2_Mtrl"));
				Animator* pAnimator = m_ChestFX->AddComponent<Animator>();
				pAnimator->Create(L"ChestOpenFX2", Resources::Load<Texture>(L"ChestFX2_Tex", L"..\\Resources\\Texture\\Map\\Stage\\ChestFX2.png")
					, Vector2::Zero, Vector2(192.f, 192.f), 5, Vector2(192.f, 192.f));
				pAnimator->CompleteEvent(L"ChestOpenFX2") = std::bind(&ChestScript::ChestOpenFXComplete, this);
				pAnimator->PlayAnimation(L"ChestOpenFX2", true);
			}
		}
	}

	void ChestScript::OnCollisionStay(Collider2D* _Other)
	{
		if (Input::GetKeyDown(eKeyCode::F) && !m_bOpen)
		{
			int iLayer = _Other->GetOwner()->GetLayerIndex();
			if ((int)eLayerType::Player == iLayer)
			{
				m_bOpen = true;
				m_Animator->PlayAnimation(L"Chest_Open", true);

				AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
				pSFX->SetClip(Resources::Load<AudioClip>(L"ChestOpenSFX", L"..\\Resources\\Sound\\SFX\\ChestOpenSFX.ogg"));
				pSFX->Play();
			}
		}
	}

	void ChestScript::CreateReward()
	{
		// 보상 생성
		GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(48.f, 48.f, 100.f), eLayerType::Item);
		pObject->SetName(L"Bow");

		MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Bow_Mtrl"));

		pObject->AddComponent<Collider2D>();

		WeaponScript* pWeaponScript = pObject->AddComponent<WeaponScript>();
		pWeaponScript->SetItemName(L"우드보우");
		pWeaponScript->SetTransform(pObject->GetComponent<Transform>());
		pWeaponScript->SetItemState(ItemScript::eItemState::Drop);
		pWeaponScript->SetWeaponType(eWeaponType::Bow);
		pWeaponScript->SetItemOption(1);
		pWeaponScript->SetDrop(true);

		// 열려 있는 애니메이션으로 변경
		m_Animator->PlayAnimation(L"Chest_Open_Stop", true);
	}

	void ChestScript::ChestOpenFXComplete()
	{
		object::Destroy(m_ChestFX);
		m_ChestFX = nullptr;

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"ForestBGM"));
		pBGM->Play();
		pBGM->SetLoop(true);
		pBGM->SetVolume(0.3f);
	}
}
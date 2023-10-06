#include "Lu_DiaryScript.h"
#include "Lu_GameObject.h"
#include "Lu_Resources.h"
#include "Lu_SceneManager.h"
#include "Lu_PlayerScript.h"
#include "Lu_Input.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Renderer.h"
#include "Lu_Camera.h"
#include "Lu_CameraScript.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"

namespace Lu
{
	DiaryScript::DiaryScript()
		: m_Animator(nullptr)
		, m_DramaFX(nullptr)
		, m_bCameraMove(false)
	{
		SetName(L"DiaryScript");
	}

	DiaryScript::~DiaryScript()
	{

	}

	void DiaryScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		CreateDiaryAnimation();

		m_Animator->PlayAnimation(L"Diary_Idle", true);
	}

	void DiaryScript::Update()
	{
		if (m_bCameraMove)
		{
			float fScale = renderer::mainCamera->GetScale();
			fScale += (float)Time::DeltaTime() / 2.f;
			renderer::mainCamera->SetScale(fScale);
		}
	}

	void DiaryScript::OnCollisionStay(Collider2D* _Other)
	{
		if (Input::GetKeyDown(eKeyCode::F))
		{
			int iLayer = _Other->GetOwner()->GetLayerIndex();
			if ((int)eLayerType::Player == iLayer)
			{
				m_Animator->PlayAnimation(L"Diary_Active", true);
				m_bCameraMove = true;
				renderer::mainCamera->GetOwner()->GetComponent<CameraScript>()->SetTarget(GetOwner());
			}
		}
	}

	void DiaryScript::CreateDiaryAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"Diary_Tex", L"..\\Resources\\Texture\\Map\\Lobby\\Diary.png");

		m_Animator->Create(L"Diary_Idle", pAtlas, Vector2(0.f, 0.f), Vector2(48.f, 36.f), 1, Vector2(48.f, 36.f));
		m_Animator->Create(L"Diary_Active", pAtlas, Vector2(0.f, 0.f), Vector2(48.f, 36.f), 8, Vector2(48.f, 36.f), Vector2::Zero, 0.2f);
		m_Animator->StartEvent(L"Diary_Active") = std::bind(&DiaryScript::DiarySFX, this);
		m_Animator->CompleteEvent(L"Diary_Active") = std::bind(&DiaryScript::GameStart, this);
	}

	void DiaryScript::GameStart()
	{
		m_Animator->PlayAnimation(L"Diary_Idle", true);
		SceneManager::LoadScene(L"WeaponChoiceScene");

		GameObject* pPlayer = SceneManager::FindPlayer();
		pPlayer->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 500.f));
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		pPlayerScript->MagicPortalMove();

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"floorStart", L"..\\Resources\\Sound\\SFX\\floorStart.ogg"));
		pSFX->Play();

		m_bCameraMove = false;

		object::Destroy(m_DramaFX);
		m_DramaFX = nullptr;
	}

	void DiaryScript::DiarySFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"DiarySFX", L"..\\Resources\\Sound\\SFX\\DiarySFX.ogg"));
		pSFX->Play();

		m_DramaFX = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
		m_DramaFX->SetName(L"DramaFX");

		MeshRenderer* pMeshRender = m_DramaFX->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Drama_Mtrl"));
	}
}
#include "Lu_DiaryScript.h"
#include "Lu_GameObject.h"
#include "Lu_Resources.h"
#include "Lu_SceneManager.h"
#include "Lu_Input.h"

namespace Lu
{
	DiaryScript::DiaryScript()
		: m_Animator(nullptr)
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

	void DiaryScript::OnCollisionStay(Collider2D* _Other)
	{
		if (Input::GetKeyDown(eKeyCode::F))
		{
			int iLayer = _Other->GetOwner()->GetLayerIndex();
			if ((int)eLayerType::Player == iLayer)
			{
				m_Animator->PlayAnimation(L"Diary_Active", true);
			}
		}
	}

	void DiaryScript::CreateDiaryAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"Diary_Tex", L"..\\Resources\\Texture\\Map\\Lobby\\Diary.png");

		m_Animator->Create(L"Diary_Idle", pAtlas, Vector2(0.f, 0.f), Vector2(48.f, 36.f), 1, Vector2(48.f, 36.f));
		m_Animator->Create(L"Diary_Active", pAtlas, Vector2(0.f, 0.f), Vector2(48.f, 36.f), 8, Vector2(48.f, 36.f), Vector2::Zero, 0.2f);
		m_Animator->CompleteEvent(L"Diary_Active") = std::bind(&DiaryScript::GameStart, this);
	}

	void DiaryScript::GameStart()
	{
		SceneManager::LoadScene(L"WeaponChoiceScene");
	}
}

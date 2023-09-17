#include "Lu_DeadState.h"
#include "Lu_Object.h"
#include "Lu_PlayerScript.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_Resources.h"

namespace Lu
{
	DeadState::DeadState()
	{
		SetName(L"DeadStateScript");
		SetStateType(eState::Dead);
	}

	DeadState::~DeadState()
	{

	}

	void DeadState::Enter()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"DeadSFX", L"..\\Resources\\Sound\\SFX\\Player\\DeadSFX.ogg"));
		pSFX->Play();
	}

	void DeadState::Exit()
	{
		object::Destroy(GetPlayerScript()->GetOwner());
		SceneManager::LoadScene(L"GameOverScene");
	}
}
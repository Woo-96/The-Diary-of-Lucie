#include "Lu_IceBallScript.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_SceneManager.h"

namespace Lu
{
	IceBallScript::IceBallScript()
	{
		SetName(L"IceBallScript");
		SetSkillType(eSkillType::IceBall);
	}

	IceBallScript::~IceBallScript()
	{
	}

	void IceBallScript::Update()
	{
		ProjectileScript::Update();
	}

	void IceBallScript::OnCollisionEnter(Collider2D* _Other)
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"IceBall2SFX", L"..\\Resources\\Sound\\SFX\\Player\\IceBall2SFX.ogg"));
		pSFX->Play();
		ProjectileScript::OnCollisionEnter(_Other);
	}

	int IceBallScript::GetProjectileDamage()
	{
		int iMin = 10;
		int iMax = 15;

		return std::rand() % (iMax - iMin + 1) + iMin;
	}
}

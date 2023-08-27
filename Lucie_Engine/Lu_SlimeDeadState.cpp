#include "Lu_SlimeDeadState.h"
#include "Lu_Object.h"
#include "Lu_SlimeScript.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"

namespace Lu
{
	SlimeDeadState::SlimeDeadState()
	{
		SetName(L"SlimeDeadStateScript");
		SetStateType(eState::Dead);

		m_SFX = new GameObject;
		m_SFX->AddComponent<AudioSource>();
	}

	SlimeDeadState::~SlimeDeadState()
	{
		if (nullptr != m_SFX)
		{
			delete m_SFX;
			m_SFX = nullptr;
		}
	}

	void SlimeDeadState::Enter()
	{
		if (!(GetSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);

		AudioSource* pAudio = m_SFX->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"SlimeDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeDeadSFX.ogg"));
		pAudio->Play();
	}

	void SlimeDeadState::Exit()
	{
		object::Destroy(GetSlimeScript()->GetOwner());
	}
}
#include "Lu_SnabyDeadState.h"
#include "Lu_Object.h"
#include "Lu_SnabyScript.h"
#include "Lu_Resources.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	SnabyDeadState::SnabyDeadState()
	{
		SetName(L"SnabyDeadStateScript");
		SetStateType(eState::Dead);

		m_SFX = new GameObject;
		m_SFX->AddComponent<AudioSource>();
	}

	SnabyDeadState::~SnabyDeadState()
	{
		if (nullptr != m_SFX)
		{
			delete m_SFX;
			m_SFX = nullptr;
		}
	}

	void SnabyDeadState::Enter()
	{
		if (!(GetSnabyScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);

		AudioSource* pAudio = m_SFX->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"SnabyDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Snaby\\SnabyDeadSFX.ogg"));
		pAudio->Play();
	}

	void SnabyDeadState::Exit()
	{
		object::Destroy(GetSnabyScript()->GetOwner());
	}
}
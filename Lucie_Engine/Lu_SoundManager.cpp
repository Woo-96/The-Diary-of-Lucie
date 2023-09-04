#include "Lu_SoundManager.h"

namespace Lu
{
	SoundManager::SoundManager()
		: m_Listener(nullptr)
		, m_BGM(nullptr)
		, m_SFX(nullptr)
	{
		SetName(L"SoundManagerScript");
	}

	SoundManager::~SoundManager()
	{
	}
}
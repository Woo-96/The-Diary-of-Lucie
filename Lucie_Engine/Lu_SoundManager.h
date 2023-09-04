#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class AudioListener;
	class AudioSource;
	class SoundManager : public Script
	{
	public:
		SoundManager();
		virtual ~SoundManager();

	private:
		AudioListener*				m_Listener;
		AudioSource*				m_BGM;
		AudioSource*				m_SFX;

	public:
		void SetListener(AudioListener* _Listener)
		{
			m_Listener = _Listener;
		}

		void SetBGM(AudioSource* _BGM)
		{
			m_BGM = _BGM;
		}

		void SetSFX(AudioSource* _SFX)
		{
			m_SFX = _SFX;
		}

	public:
		AudioListener* GetListener()	const
		{
			return m_Listener;
		}

		AudioSource* GetBGM()	const
		{
			return m_BGM;
		}

		AudioSource* GetSFX()	const
		{
			return m_SFX;
		}
	};
}
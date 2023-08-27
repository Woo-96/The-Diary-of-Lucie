#include "Lu_AudioClip.h"
#include "Lu_Transform.h"

namespace Lu
{
	AudioClip::AudioClip()
		: Resource(eResourceType::AudioClip)
		, m_Sound(nullptr)
		, m_Channel(nullptr)
		, m_MinDistance(1.0f)
		, m_MaxDistance(1000.0f)
		, m_bLoop(false)
	{

	}

	AudioClip::~AudioClip()
	{
	}

	HRESULT AudioClip::Load(const std::wstring& _Path)
	{
		std::string cPath(_Path.begin(), _Path.end());
		if (!Fmod::CreateSound(cPath, &m_Sound))
			return S_FALSE;

		m_Sound->set3DMinMaxDistance(m_MinDistance, m_MaxDistance);

		return S_OK;
	}

	void AudioClip::Play()
	{
		if (m_bLoop)
			m_Sound->setMode(FMOD_LOOP_NORMAL);
		else
			m_Sound->setMode(FMOD_LOOP_OFF);

		Fmod::SoundPlay(m_Sound, &m_Channel);
	}

	void AudioClip::Stop()
	{
		m_Channel->stop();
	}

	void AudioClip::SetVolume(float _Volume)
	{
		m_Channel->setVolume(_Volume);
	}

	void AudioClip::Set3DAttributes(const Vector3 _Pos, const Vector3 _Vel)
	{
		FMOD_VECTOR fmodPos(_Pos.x, _Pos.y, _Pos.z);
		FMOD_VECTOR fmodVel(_Vel.x, _Vel.y, _Vel.z);

		m_Channel->set3DAttributes(&fmodPos, &fmodVel);
	}
}

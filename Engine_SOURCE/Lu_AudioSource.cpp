#include "Lu_AudioSource.h"
#include "Lu_AudioClip.h"
#include "Lu_Transform.h"
#include "Lu_GameObject.h"

namespace Lu
{
	AudioSource::AudioSource()
		: Component(eComponentType::AudioSource)
		, m_AudioClip(nullptr)
	{

	}

	AudioSource::~AudioSource()
	{

	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::LateUpdate()
	{
		//Transform* tr = GetOwner()->GetComponent<Transform>();
		//Vector3 pos = tr->GetPosition();
		//Vector3 foward = tr->Foward();

		//m_AudioClip->Set3DAttributes(pos, foward);
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play()
	{
		m_AudioClip->Play();
	}
	void AudioSource::Stop()
	{
		m_AudioClip->Stop();
	}
	void AudioSource::SetLoop(bool loop)
	{
		m_AudioClip->SetLoop(loop);
	}

	void AudioSource::SetVolume(float _Volume)
	{
		m_AudioClip->SetVolume(_Volume);
	}
}
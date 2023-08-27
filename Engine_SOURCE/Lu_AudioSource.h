#pragma once
#include "Lu_Component.h"
#include "Lu_AudioClip.h"

namespace Lu
{
	class AudioSource : public Component
	{
	public:
		AudioSource();
		virtual ~AudioSource();

	private:
		std::shared_ptr<AudioClip> m_AudioClip;

	public:
		void SetClip(std::shared_ptr<AudioClip> _Clip) 
		{ 
			m_AudioClip = _Clip;
		}

		std::shared_ptr<AudioClip> GetClip() 
		{
			return m_AudioClip;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		void Play();
		void Stop();
		void SetLoop(bool loop);
		// 0 ~ 1
		void SetVolume(float _Volume);		
	};
}

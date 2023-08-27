#pragma once
#include "Lu_Resource.h"
#include "Lu_Fmod.h"



//#include <Fmod/fmod_studio.hpp>
//#include <Fmod/fmod.hpp>
//#include <Fmod/fmod_common.h>
//#include <Fmod/fmod_codec.h>

//#include "..\External\Include\\DirectXTex\DirectXTex.h"

namespace Lu
{
	using namespace Lu::math;
	class AudioClip : public Resource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

	private:
		FMOD::Sound*		m_Sound;
		FMOD::Channel*		m_Channel;
		float				m_MinDistance;
		float				m_MaxDistance;
		bool				m_bLoop;

	public:
		void Play();
		void Stop();
		void SetVolume(float _Volume);		// 0 ~ 1
		void Set3DAttributes(const Vector3 _Pos, const Vector3 _Vel);
		void SetLoop(bool _Loop) 
		{ 
			m_bLoop = _Loop;
		}

	public:
		virtual HRESULT Load(const std::wstring& _Path) override;
	};
}


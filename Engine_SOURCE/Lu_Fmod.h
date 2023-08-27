#pragma once
#include "Lucie_Engine.h"
#include "Lu_Math.h"

#include "..\\External\\Fmod\\Include\\fmod_studio.hpp"
#include "..\\External\\Fmod\\Include\\fmod.hpp"
#include "..\\External\\Fmod\\Include\\fmod_common.h"
#include "..\\External\\Fmod\\Include\\fmod_codec.h"
//#include "..\\External\\Include\\Fmod\\common_platform.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Fmod\\Library\\Debug\\fmodL_vc.lib")
#pragma comment(lib, "..\\External\\Fmod\\Library\\Debug\\fmodstudioL_vc.lib")
#else
#pragma comment(lib, "..\\External\\Fmod\\Library\\Debug\\fmod_vc.lib")
#pragma comment(lib, "..\\External\\Fmod\\Library\\Release\\fmodstudio_vc.lib")
#endif

namespace Lu
{
	using namespace math;

	class Fmod
	{
	private:
		static FMOD::Studio::System*	m_System;
		static FMOD::System*			m_CoreSystem;

	public:
		static void Initialize();
		static bool CreateSound(const std::string& _Path, FMOD::Sound** _Sound);
		static void SoundPlay(FMOD::Sound* _Sound, FMOD::Channel** _Channel);
		static void Set3DListenerAttributes(const Vector3* _Pos, const Vector3* _Vel, const Vector3* _Forward, const Vector3* _Up);
		static void Release();
	};
}

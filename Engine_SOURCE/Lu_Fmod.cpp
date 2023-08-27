#include "Lu_Fmod.h"
#include <combaseapi.h>


namespace Lu
{
	FMOD::Studio::System* Fmod::m_System = nullptr;
	FMOD::System* Fmod::m_CoreSystem = nullptr;

	void Fmod::Initialize()
	{
		void* extraDriverData = NULL;
		FMOD::Studio::System::create(&m_System);

		m_System->getCoreSystem(&m_CoreSystem);
		m_CoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
		
		m_System->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
	}

	bool Fmod::CreateSound(const std::string& _Path,  FMOD::Sound** _Sound)
	{
		if (FMOD_OK != m_CoreSystem->createSound(_Path.c_str(), FMOD_3D, 0, _Sound))
			return false;

		return true;
	}

	void Fmod::SoundPlay(FMOD::Sound* _Sound, FMOD::Channel** _Channel)
	{
		m_CoreSystem->playSound(_Sound, 0, false, _Channel);
	}

	void Fmod::Set3DListenerAttributes(const Vector3* _Pos, const Vector3* _Vel, const Vector3* _Forward, const Vector3* _Up)
	{
		FMOD_VECTOR fmodPos(_Pos->x, _Pos->y, _Pos->z);
		FMOD_VECTOR fmodVel(_Vel->x, _Vel->y, _Vel->z);
		FMOD_VECTOR fmodForward(_Forward->x, _Forward->y, _Forward->z);
		FMOD_VECTOR fmodUp(_Up->x, _Up->y, _Up->z);

		m_CoreSystem->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);
	}

	void Fmod::Release()
	{
		m_System->release();
		m_System = nullptr;
	}
}

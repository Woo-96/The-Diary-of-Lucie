#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_ScriptEnums.h"

namespace Lu
{
    class PortalScript : public Script
    {
    public:
        PortalScript();
        virtual ~PortalScript();

	private:
		std::wstring	m_CurSceneName;
		std::wstring	m_NextSceneName;
		Vector2			m_SpawnPos;

	public:
		void SetCurSceneName(std::wstring _CurSceneName)
		{
			m_CurSceneName = _CurSceneName;
		}

		void SetNextSceneName(std::wstring _NextSceneName)
		{
			m_NextSceneName = _NextSceneName;
		}

	public:
		virtual void Initialize() override;

	public:
		virtual void OnCollisionStay(Collider2D* _Other);
    };
}
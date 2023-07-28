#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "..\\Engine_SOURCE\\Lu_Camera.h"

namespace Lu
{
	class MouseScript : public Script
	{
	public:
		MouseScript();
		virtual ~MouseScript();

	private:
		Camera* m_UICam;
		Vector2 m_OffsetPos;
		//Vector2	m_MousePos;
		//Vector2	m_Resolution;
		//Vector2	m_MouseMove;
		//bool	m_MouseLDown;
		//bool	m_MouseLPush;
		//bool	m_MouseLUp;

	public:
		void SetUICam(Camera* _UICam)
		{
			m_UICam = _UICam;
		}

	public:
		virtual void Update() override;
		virtual void LateUpdate() override;
	};
}

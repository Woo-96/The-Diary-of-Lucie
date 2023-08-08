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
		Camera* m_Cam;
		Vector2 m_OffsetPos;

	public:
		void SetCam(Camera* _Cam)
		{
			m_Cam = _Cam;
		}

	public:
		virtual void Update() override;
		virtual void LateUpdate() override;
	};
}

#pragma once
#include "..\\Engine_SOURCE\Lu_Script.h"

namespace Lu
{
	class CameraScript : public Script
	{
	public:
		CameraScript();
		virtual ~CameraScript();

	private:
		float m_Speed;

	public:
		virtual void Update() override;
	};
}
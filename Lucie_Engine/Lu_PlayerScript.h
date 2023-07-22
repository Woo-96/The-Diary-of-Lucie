#pragma once
//#include "Lu_Script.h"
#include <Lu_Script.h>

namespace Lu
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

	private:
		float m_Speed;

	public:
		virtual void Initialize() override;
		virtual void Update() override;
	};
}



#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class TitleBtn : public UIScript
	{
		enum class eTitleState
		{
			NONE,
			NEWGAME,
			CONTINUE,
			EXIT,
			END,
		};

	public:
		TitleBtn();
		virtual ~TitleBtn();

	private:
		eTitleState m_State;

	public:
		virtual void Update() override;
	};
}



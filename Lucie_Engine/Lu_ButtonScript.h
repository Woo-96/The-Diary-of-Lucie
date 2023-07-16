#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class ButtonScript : public UIScript
	{
		enum class eButtonState
		{
			None,
			MouseHovered,
			Click,
			Disable,
			END,
		};

	public:
		ButtonScript();
		virtual ~ButtonScript();

	private:
		eButtonState m_PrevButton;
		eButtonState m_CurButton;

	public:
		virtual void Update() override;

	protected:
		virtual void ChangeButtonTex() = 0;
	};
}



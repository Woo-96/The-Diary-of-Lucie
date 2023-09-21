#pragma once
#include "Lu_UIScript.h"
#include "Lu_Material.h"

namespace Lu
{
	class ButtonScript : public UIScript
	{
	public:
		enum class eButtonState
		{
			Normal,
			MouseHovered,
			Click,
			Disable,
			End,
		};

	public:
		ButtonScript();
		virtual ~ButtonScript();

	private:
		eButtonState m_CurBtnState;
		eButtonState m_PrevBtnState;

	public:
		eButtonState GetCurBtnState()	const
		{
			return m_CurBtnState;
		}

		eButtonState GetPrevBtnState()	const
		{
			return m_PrevBtnState;
		}

	public:
		virtual void Update() override;

	private:
		bool MouseCollision();

	protected:
		virtual void ButtonClickEvent() = 0;
	};
}



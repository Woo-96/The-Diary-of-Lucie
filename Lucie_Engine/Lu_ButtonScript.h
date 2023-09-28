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
		virtual void Render() override {}

	private:
		bool MouseCollision();

	protected:
		virtual void ButtonNomalEvent() = 0;
		virtual void ButtonHoveredEvent() = 0;
		virtual void ButtonClickEvent() = 0;
	};
}



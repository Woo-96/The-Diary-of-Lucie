#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class ButtonScript : public UIScript
	{
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
		eButtonState m_ButtonState;

		Vector2      m_arrLT[(int)eButtonState::End];
		Vector2      m_SliceSize;
		Vector2      m_TexResolution;

		std::function<void()>   m_Callback[(int)eButtonState::End];
		//std::shared_ptr<Sound>    m_StateSound[(int)eButtonState::End];

	public:
		void SetButtonLT(Vector2 _arrLT[], int _Count);

		void SetSliceSize(Vector2 _SliceSize)
		{
			m_SliceSize = _SliceSize;
		}

		void SetTexResolution(Vector2 _TexResolution)
		{
			m_TexResolution = _TexResolution;
		}

		// 버튼 비활성화 (안눌리는 버튼)
		void EnableButton(bool Enable)
		{
			m_ButtonState = Enable ? eButtonState::Normal : eButtonState::Disable;
		}

	public:
		virtual void Update() override;

	public:
		void Binds();

	public:
		virtual void CollisionMouseHoveredCallback(const Vector2& Pos);
		virtual void CollisionMouseReleaseCallback();

	public:
		template <typename T>
		void SetCallback(eButtonState State, T* Obj, void(T::* Func)())
		{
			m_Callback[(int)State] = std::bind(Func, Obj);
		}
	};
}



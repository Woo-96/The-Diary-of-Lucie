#pragma once
#include "Lucie_Engine.h"

namespace Lu
{
	using namespace Lu::math;
	class GameObject;

	enum class eKeyCode
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,

		_1, _2, _3, _4, _5, _6, _7, _8, _9, _0,

		UP, DOWN, LEFT, RIGHT, SPACE,
		LBUTTON, RBUTTON,
		CTRL, ALT, ENTER, SHIFT,

		LEFT_BRACKET, RIGHT_BRACKET,
		END,
	};

	enum class eKeyState
	{
		Down,
		Pressed,
		Up,
		None,
	};

	class Input
	{
	public:
		struct Key
		{
			eKeyCode	eKey;
			eKeyState	eState;
			bool		bPressed;
		};

	private:
		static std::vector<Key>		m_Keys;
		static Vector2				m_MousePos;

	public:
		inline static eKeyState GetKeyState(eKeyCode keyCode)
		{
			return m_Keys[(UINT)keyCode].eState;
		};

		// 키를 누르는 시간만큼 true를 반환
		static __forceinline bool GetKey(eKeyCode keyCode)
		{
			return m_Keys[static_cast<UINT>(keyCode)].eState == eKeyState::Pressed;
		}

		// 키를 눌렀을 때, 딱 한번 true를 반환
		static __forceinline bool GetKeyDown(eKeyCode keyCode)
		{
			return m_Keys[static_cast<UINT>(keyCode)].eState == eKeyState::Down;
		}

		// 키를 누르다 땠을 때, 딱 한번 true를 반환
		static __forceinline bool GetKeyUp(eKeyCode keyCode)
		{
			return m_Keys[static_cast<UINT>(keyCode)].eState == eKeyState::Up;
		}

		static __forceinline Vector2 GetMousePos()
		{
			return m_MousePos;
		}

	public:
		static void Initialize();
		static void Update();
		static void Render(HDC _hDC);
	};
}

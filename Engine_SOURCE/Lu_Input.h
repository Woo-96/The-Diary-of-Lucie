#pragma once
#include "Lucie_Engine.h"

namespace Lu
{
	using namespace Lu::math;

	enum class eKeyCode
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,

		UP, DOWN, LEFT, RIGHT, SPACE,
		LBUTTON, RBUTTON,
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

		// ========================================
		Vector2						m_MouseWorldPos;	// 월드공간에서의 마우스 위치
		Vector2						m_MouseMove;
		bool						m_MouseLDown;
		bool						m_MouseLPush;
		bool						m_MouseLUp;
		//CollisionProfile*			m_MouseProfile;		// 마우스 충돌 필요한가?
		class CGameObject*			m_MouseObj;
		bool						m_ShowCursor;

	public:
		bool GetMouseLDown()	const
		{
			return m_MouseLDown;
		}

		bool GetMouseLPush()	const
		{
			return m_MouseLPush;
		}

		bool GetMouseLUp()	const
		{
			return m_MouseLUp;
		}

		const Vector2& GetMouseWorldPos()	const
		{
			return m_MouseWorldPos;
		}

		const Vector2& GetMouseMove()	const
		{
			return m_MouseMove;
		}

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

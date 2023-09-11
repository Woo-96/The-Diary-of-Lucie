#include "Lu_Input.h"
#include "Lu_Application.h"
#include "Lu_Renderer.h"

extern Lu::Application application;

namespace Lu
{
	int ASCII[(UINT)eKeyCode::END] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',

		VK_UP, VK_DOWN, VK_LEFT ,VK_RIGHT, VK_SPACE,
		VK_LBUTTON, VK_RBUTTON,
		VK_CONTROL, VK_MENU, VK_RETURN, VK_SHIFT,

		VK_OEM_4, VK_OEM_6,
	};

	std::vector<Input::Key> Input::m_Keys;
	Vector2 Input::m_MousePos = Vector2::Zero;

	void Input::Initialize()
	{
		for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
		{
			Key keyInfo;
			keyInfo.eKey = (eKeyCode)i;
			keyInfo.eState = eKeyState::None;
			keyInfo.bPressed = false;

			m_Keys.push_back(keyInfo);
		}

		ShowCursor(FALSE);
	}

	void Input::Update()
	{
		if (GetFocus())
		{
			for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
			{
				if (GetAsyncKeyState(ASCII[i]) & 0x8000)
				{
					// 이전 프레임에도 눌려 있었다
					if (m_Keys[i].bPressed)
						m_Keys[i].eState = eKeyState::Pressed;
					else
						m_Keys[i].eState = eKeyState::Down;

					m_Keys[i].bPressed = true;
				}
				else // 현재 프레임에 키가 눌려있지 않다.
				{
					// 이전 프레임에 내키가 눌려있엇다.
					if (m_Keys[i].bPressed)
						m_Keys[i].eState = eKeyState::Up;
					else
						m_Keys[i].eState = eKeyState::None;

					m_Keys[i].bPressed = false;
				}
			}

			POINT mousePos = {};
			GetCursorPos(&mousePos);

			ScreenToClient(application.GetHwnd(), &mousePos);
			m_MousePos.x = (float)mousePos.x;
			m_MousePos.y = (float)mousePos.y;
		}
		else
		{
			for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
			{
				if (eKeyState::Down == m_Keys[i].eState
					|| eKeyState::Pressed == m_Keys[i].eState)
				{
					m_Keys[i].eState = eKeyState::Up;
				}
				else if (eKeyState::Up == m_Keys[i].eState)
				{
					m_Keys[i].eState = eKeyState::None;
				}

				m_Keys[i].bPressed = false;
			}
		}
	}

	void Input::Render(HDC _hDC)
	{

	}
}

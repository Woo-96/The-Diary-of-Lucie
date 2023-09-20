#include "Lu_ButtonScript.h"
#include "Lu_Input.h"
#include "Lu_GameObject.h"
#include "Lu_Renderer.h"
#include "Lu_SceneManager.h"
#include "Lu_Scene.h"
#include "Lu_Camera.h"

namespace Lu
{
	ButtonScript::ButtonScript()
		: m_CurBtnState(eButtonState::Normal)
		, m_PrevBtnState(eButtonState::Normal)
	{
		SetName(L"ButtonScript");
	}

	ButtonScript::~ButtonScript()
	{

	}

	void ButtonScript::Update()
	{
		m_PrevBtnState = m_CurBtnState;

		if ((int)m_CurBtnState > (int)eButtonState::Disable)
			return;

		if ((int)m_CurBtnState != (int)eButtonState::Disable)
		{
			if (MouseCollision())
			{
				if (Input::GetKeyUp(eKeyCode::LBUTTON))
				{
					m_CurBtnState = eButtonState::Click;
				}
				else
				{
					m_CurBtnState = eButtonState::MouseHovered;
				}
			}
			else
			{
				m_CurBtnState = eButtonState::Normal;
			}
		}
		else
		{
			m_CurBtnState = eButtonState::Normal;
		}
	}

	bool ButtonScript::MouseCollision()
	{
		Transform* pTransform = GetOwner()->GetComponent<Transform>();
		Vector3 vButtonWorldPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 vButtonScreenPos = SceneManager::GetActiveScene()->GetUICam()->WorldToScreen(vButtonWorldPos);
		Vector3 vButtonSize = GetOwner()->GetComponent<Transform>()->GetScale();

		Vector2 vMousePos = Input::GetMousePos();

		float fLeft = vButtonScreenPos.x - vButtonSize.x / 2.f;
		float fRight = vButtonScreenPos.x + vButtonSize.x / 2.f;
		float fTop = vButtonScreenPos.y + vButtonSize.y / 2.f;
		float fBottom = vButtonScreenPos.y - vButtonSize.y / 2.f;

		if (vMousePos.x < fLeft)
			return false;

		else if (vMousePos.y < fBottom)
			return false;

		else if (vMousePos.x > fRight)
			return false;

		else if (vMousePos.y > fTop)
			return false;

		return true;
	}
}

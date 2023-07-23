#include "Lu_StateScript.h"
#include "Lu_Input.h"
#include "Lu_Renderer.h"
#include "Lu_Transform.h"
#include "Lu_PlayerScript.h"
#include "Lu_GameObject.h"

namespace Lu
{
	StateScript::StateScript()
		: m_StateType(eState::End)
		, m_Owner(nullptr)
		, m_Transform(nullptr)
		, m_ActionSpeed(0.f)
		, m_ActionDir(Vector3::Zero)
	{
		SetName(L"StateScript");
	}

	StateScript::~StateScript()
	{

	}

	void StateScript::Initialize()
	{
		m_Transform = GetOwner()->GetOwner()->GetComponent<Transform>();
	}

	void StateScript::CalDirToMouse()
	{
		Vector3 vPos = m_Transform->GetPosition();
		Vector2 vScreenPos = Lu::Input::GetMousePos();
		Vector3 vMouseWorldPos = renderer::mainCamera->ScreenToWorld(vScreenPos);

		m_ActionDir = vMouseWorldPos - vPos;
		m_ActionDir.Normalize();
	}
}
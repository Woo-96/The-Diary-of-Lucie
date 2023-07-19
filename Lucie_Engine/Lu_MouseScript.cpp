#include "Lu_MouseScript.h"
#include "Lu_Input.h"
#include "Lu_GameObject.h"
#include "Lu_Transform.h"
#include "Lu_Camera.h"

namespace Lu
{
	MouseScript::MouseScript()
		: m_UICam(nullptr)
		, m_OffsetPos(Vector2(21.f, -23.f))
	{
		SetName(L"MouseScript");
	}

	MouseScript::~MouseScript()
	{

	}

	void MouseScript::Update()
	{
		
	}

	void MouseScript::LateUpdate()
	{
		Transform* pTransform = GetOwner()->GetComponent<Transform>();
		Vector2 vScreenPos = Lu::Input::GetMousePos();
		Vector3 vWorldPos = m_UICam->ScreenToWorld(vScreenPos);
		pTransform->SetPosition(Vector3(vWorldPos.x + m_OffsetPos.x, vWorldPos.y + m_OffsetPos.y, 0.f));
	}
}
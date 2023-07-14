#include "Lu_CameraScript.h"
#include "Lu_Transform.h"
#include "Lu_GameObject.h"
#include "Lu_Time.h"
#include "Lu_Input.h"

namespace Lu
{
	CameraScript::CameraScript()
		: m_Speed(500.f)
	{

	}

	CameraScript::~CameraScript()
	{

	}

	void CameraScript::Update()
	{
		Transform* pTransform = GetOwner()->GetComponent<Transform>();
		Vector3 vPos = pTransform->GetPosition();

		if (Input::GetKey(eKeyCode::LEFT))
		{
			vPos.x -= m_Speed * Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::RIGHT))
		{
			vPos.x += m_Speed * Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::DOWN))
		{
			vPos.y -= m_Speed * Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::UP))
		{
			vPos.y += m_Speed * Time::DeltaTime();
		}

		pTransform->SetPosition(vPos);
	}
}
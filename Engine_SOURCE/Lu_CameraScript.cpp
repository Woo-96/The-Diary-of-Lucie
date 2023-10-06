#include "Lu_CameraScript.h"
#include "Lu_Transform.h"
#include "Lu_GameObject.h"
#include "Lu_Time.h"
#include "Lu_Input.h"
#include "Lu_Camera.h"
#include "Lu_Application.h"

extern Lu::Application application;

namespace Lu
{
	CameraScript::CameraScript()
		: m_Speed(500.f)
		, m_WindowResolution(Vector2::Zero)
		, m_WorldResolution(Vector2::Zero)
		, m_Target(nullptr)
		, m_Offset(Vector2::Zero)
	{
		SetName(L"CameraScript");
	}

	CameraScript::~CameraScript()
	{

	}

	void CameraScript::SetTarget(GameObject* _Target)
	{
		m_Target = _Target;

		m_Speed = 3.f;

		if (m_Target != nullptr)
		{
			Transform* pCameraTransform = GetOwner()->GetComponent<Transform>();
			Vector3 vTargetPos = m_Target->GetComponent<Transform>()->GetPosition();
			vTargetPos.x += m_Offset.x;
			vTargetPos.y += m_Offset.y;

			// 카메라 위치 설정
			pCameraTransform->SetPosition(vTargetPos);
		}
	}

	void CameraScript::Initialize()
	{
		m_WindowResolution = Vector2((float)application.GetWidth(), (float)application.GetHeight());
	}

	void CameraScript::Update()
	{
		if (nullptr == m_Target)
		{
			Transform* pTransform = GetOwner()->GetComponent<Transform>();
			Vector3 vPos = pTransform->GetPosition();

			if (Input::GetKey(eKeyCode::LEFT))
			{
				vPos.x -= m_Speed * (float)Time::DeltaTime();
			}
			else if (Input::GetKey(eKeyCode::RIGHT))
			{
				vPos.x += m_Speed * (float)Time::DeltaTime();
			}
			else if (Input::GetKey(eKeyCode::DOWN))
			{
				vPos.y -= m_Speed * (float)Time::DeltaTime();
			}
			else if (Input::GetKey(eKeyCode::UP))
			{
				vPos.y += m_Speed * (float)Time::DeltaTime();
			}

			Camera* pCamera = GetOwner()->GetComponent<Camera>();

			if (Input::GetKey(eKeyCode::Z))
			{
				float fScale = pCamera->GetScale();
				fScale += (float)Time::DeltaTime();
				pCamera->SetScale(fScale);
			}

			if (Input::GetKey(eKeyCode::X))
			{
				float fScale = pCamera->GetScale();
				fScale -= (float)Time::DeltaTime();
				pCamera->SetScale(fScale);
			}

			pTransform->SetPosition(vPos);
		}

		else
		{
			Vector3 vTargetPos = m_Target->GetComponent<Transform>()->GetPosition();
			vTargetPos.x += m_Offset.x;
			vTargetPos.y += m_Offset.y;
			Vector3 vPos = GetOwner()->GetComponent<Transform>()->GetPosition();
			Vector3 vMoveDist = vTargetPos - vPos;
			if (abs(vMoveDist.x) <= 30.f && abs(vMoveDist.y) <= 30.f)
			{
				vMoveDist.x = 0.f;
				vMoveDist.y = 0.f;
			}

			vPos.x += vMoveDist.x * m_Speed * (float)Time::DeltaTime();
			vPos.y += vMoveDist.y * m_Speed * (float)Time::DeltaTime();

			if (0.f != m_WorldResolution.x && 0.f != m_WorldResolution.y)
			{
				float Left = -m_WorldResolution.x / 2.f;
				float Right = m_WorldResolution.x / 2.f;
				float Top = m_WorldResolution.y / 2.f;
				float Bottom = -m_WorldResolution.y / 2.f;

				Vector2 vDiff_RT = Vector2(Right - (vPos.x + m_WindowResolution.x / 2),
					Top - (vPos.y + m_WindowResolution.y / 2));
				Vector2 vDiff_LB = Vector2(Left - (vPos.x - m_WindowResolution.x / 2),
					Bottom - (vPos.y - m_WindowResolution.y / 2));

				if (vDiff_RT.x <= 0.f)
				{
					vPos.x = Right - (m_WindowResolution.x / 2);;
				}

				if (vDiff_LB.x >= 0.f)
				{
					vPos.x = Left + (m_WindowResolution.x / 2);
				}

				if (vDiff_RT.y <= 0.f)
				{
					vPos.y = Top - (m_WindowResolution.y / 2);
				}

				if (vDiff_LB.y >= 0.f)
				{
					vPos.y = Bottom + (m_WindowResolution.y / 2);
				}
			}

			GetOwner()->GetComponent<Transform>()->SetPosition(vPos);
		}
	}
}
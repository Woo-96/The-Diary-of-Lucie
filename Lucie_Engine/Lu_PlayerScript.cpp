#include "Lu_PlayerScript.h"
#include "Lu_Animator.h"
#include "Lu_GameObject.h"
#include "Lu_Input.h"
#include "Lu_Time.h"

namespace Lu
{
	PlayerScript::PlayerScript()
		: m_Speed(500.f)
	{
		SetName(L"PlayerScript");
	}

	PlayerScript::~PlayerScript()
	{

	}

	void PlayerScript::Initialize()
	{
		Animator* at = GetOwner()->GetComponent<Animator>();
		//at->CompleteEvent(L"Idle") = std::bind(&PlayerScript::Complete, this);
	}

	void PlayerScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		if (Input::GetKey(eKeyCode::A))
		{
			pos.x -= m_Speed * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::D))
		{
			pos.x += m_Speed * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::S))
		{
			pos.y -= m_Speed * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::W))
		{
			pos.y += m_Speed * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
	}
}
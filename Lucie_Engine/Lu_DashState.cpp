#include "Lu_DashState.h"
#include "Lu_Time.h"
#include "Lu_PlayerScript.h"
#include "Lu_Input.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_Resources.h"
#include "Lu_SceneManager.h"

namespace Lu
{
	DashState::DashState()
	{
		SetName(L"DashStateScript");
		SetStateType(eState::Dash);
	}

	DashState::~DashState()
	{

	}

	void DashState::Initialize()
	{
		StateScript::Initialize();

		SetSpeed(400.f);
	}

	void DashState::Update()
	{
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;
		vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);
	}

	void DashState::Enter()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"DashSFX", L"..\\Resources\\Sound\\SFX\\Player\\DashSFX.ogg"));
		pSFX->Play();

		GetPlayerScript()->UseStamina(20.f);
		GetPlayerScript()->SetAction(true);

		// 방향 결정
		if (Input::GetKey(eKeyCode::A)
			&& Input::GetKey(eKeyCode::D))
		{
			CalDirToMouse();
		}
		else if (Input::GetKey(eKeyCode::W)
			&& Input::GetKey(eKeyCode::S))
		{
			CalDirToMouse();
		}
		else if (Input::GetKey(eKeyCode::A)
			&& Input::GetKey(eKeyCode::W))
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			SetDir(Vector3(-diagonalFactor, diagonalFactor, 1.f));
		}
		else if (Input::GetKey(eKeyCode::A)
			&& Input::GetKey(eKeyCode::S))
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			SetDir(Vector3(-diagonalFactor, -diagonalFactor, 1.f));
		}
		else if (Input::GetKey(eKeyCode::D)
			&& Input::GetKey(eKeyCode::S))
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			SetDir(Vector3(diagonalFactor, -diagonalFactor, 1.f));
		}
		else if (Input::GetKey(eKeyCode::D)
			&& Input::GetKey(eKeyCode::W))
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			SetDir(Vector3(diagonalFactor, diagonalFactor, 1.f));
		}
		else if (Input::GetKey(eKeyCode::A))
		{
			SetDir(Vector3(-1.f, 0.f, 1.f));
		}
		else if (Input::GetKey(eKeyCode::D))
		{
			SetDir(Vector3(1.f, 0.f, 1.f));
		}
		else if (Input::GetKey(eKeyCode::S))
		{
			SetDir(Vector3(0.f, -1.f, 1.f));
		}
		else if (Input::GetKey(eKeyCode::W))
		{
			SetDir(Vector3(0.f, 1.f, 1.f));
		}
		else
		{
			CalDirToMouse();
		}
	}

	void DashState::Exit()
	{
		GetPlayerScript()->SetAction(false);
	}
}
#include "Lu_MoveState.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	MoveState::MoveState()
	{
		SetName(L"MoveStateScript");
		SetStateType(eState::Move);
	}

	MoveState::~MoveState()
	{

	}

	void MoveState::Initialize()
	{
        StateScript::Initialize();
	}

    void MoveState::Update()
    {
        float fCurSpeed;

        if (PlayerScript::eMoveType::Walk == GetOwner()->GetMoveType())
            fCurSpeed = 200.f;
        else
            fCurSpeed = 400.f;

        PlayerScript::ePlayerDir eCurDir = GetOwner()->GetDir();
        Vector3 vPos = GetTransform()->GetPosition();

        float diagonalFactor = 1.0f / sqrt(2.0f); // Normalize the diagonal movement vector

        switch (eCurDir)
        {
        case Lu::PlayerScript::ePlayerDir::Left:
            vPos.x -= fCurSpeed * (float)Time::DeltaTime();
            break;
        case Lu::PlayerScript::ePlayerDir::Right:
            vPos.x += fCurSpeed * (float)Time::DeltaTime();
            break;
        case Lu::PlayerScript::ePlayerDir::Up:
            vPos.y += fCurSpeed * (float)Time::DeltaTime();
            break;
        case Lu::PlayerScript::ePlayerDir::Down:
            vPos.y -= fCurSpeed * (float)Time::DeltaTime();
            break;
        case Lu::PlayerScript::ePlayerDir::LeftUp:
        {
            vPos.x -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        case Lu::PlayerScript::ePlayerDir::RightUp:
        {
            vPos.x += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        case Lu::PlayerScript::ePlayerDir::LeftDown:
        {
            vPos.x -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        case Lu::PlayerScript::ePlayerDir::RightDown:
        {
            vPos.x += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        }

        GetTransform()->SetPosition(vPos);
    }


	void MoveState::Enter()
	{

	}

	void MoveState::Exit()
	{

	}
}
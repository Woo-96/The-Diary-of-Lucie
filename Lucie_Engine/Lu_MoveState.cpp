#include "Lu_MoveState.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"
#include "Lu_AudioSource.h"
#include "Lu_SceneManager.h"
#include "Lu_Resources.h"
#include "Lu_SoundManager.h"

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

        if (PlayerScript::eMoveType::Walk == GetPlayerScript()->GetMoveType())
            fCurSpeed = 200.f;
        else
            fCurSpeed = 400.f;

        eDir eCurDir = GetPlayerScript()->GetDir();
        Vector3 vPos = GetTransform()->GetPosition();

        float diagonalFactor = 1.0f / sqrt(2.0f);

        switch (eCurDir)
        {
        case eDir::Left:
            vPos.x -= fCurSpeed * (float)Time::DeltaTime();
            break;
        case eDir::Right:
            vPos.x += fCurSpeed * (float)Time::DeltaTime();
            break;
        case eDir::Up:
            vPos.y += fCurSpeed * (float)Time::DeltaTime();
            break;
        case eDir::Down:
            vPos.y -= fCurSpeed * (float)Time::DeltaTime();
            break;
        case eDir::LeftUp:
        {
            vPos.x -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        case eDir::RightUp:
        {
            vPos.x += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        case eDir::LeftDown:
        {
            vPos.x -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        case eDir::RightDown:
        {
            vPos.x += fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
            vPos.y -= fCurSpeed * diagonalFactor * (float)Time::DeltaTime();
        }
        break;
        }

        GetTransform()->SetPosition(vPos);

        bool b = GetPlayerScript()->IsWoodGround();
        if (m_bWood != b)
        {
            m_bWood = b;
            if (m_SFX)
                m_SFX->Stop();

            if (m_bWood)
            {
                m_SFX = Resources::Load<AudioClip>(L"WoodRunSFX", L"..\\Resources\\Sound\\SFX\\Player\\WoodRunSFX.ogg");
                m_SFX->Play();
                m_SFX->SetVolume(0.3f);
                m_SFX->SetLoop(true);
            }
            else
            {
                if (PlayerScript::eMoveType::Walk == GetPlayerScript()->GetMoveType())
                {
                    m_SFX = Resources::Load<AudioClip>(L"GrassWalkSFX", L"..\\Resources\\Sound\\SFX\\Player\\GrassWalkSFX.ogg");
                    m_SFX->Play();
                    m_SFX->SetVolume(0.5f);
                    m_SFX->SetLoop(true);
                }
                else
                {
                    m_SFX = Resources::Load<AudioClip>(L"GrassRunSFX", L"..\\Resources\\Sound\\SFX\\Player\\GrassRunSFX.ogg");
                    m_SFX->Play();
                    m_SFX->SetVolume(0.5f);
                    m_SFX->SetLoop(true);
                }
            }
        }
    }


	void MoveState::Enter()
	{
        m_bWood = GetPlayerScript()->IsWoodGround();

        if (m_bWood)
        {
            m_SFX = Resources::Load<AudioClip>(L"WoodRunSFX", L"..\\Resources\\Sound\\SFX\\Player\\WoodRunSFX.ogg");
            m_SFX->Play();
            m_SFX->SetVolume(0.3f);
            m_SFX->SetLoop(true);
        }
        else
        {
            if (PlayerScript::eMoveType::Walk == GetPlayerScript()->GetMoveType())
            {
                m_SFX = Resources::Load<AudioClip>(L"GrassWalkSFX", L"..\\Resources\\Sound\\SFX\\Player\\GrassWalkSFX.ogg");
                m_SFX->Play();
                m_SFX->SetVolume(0.5f);
                m_SFX->SetLoop(true);
            }
            else
            {
                m_SFX = Resources::Load<AudioClip>(L"GrassRunSFX", L"..\\Resources\\Sound\\SFX\\Player\\GrassRunSFX.ogg");
                m_SFX->Play();
                m_SFX->SetVolume(0.5f);
                m_SFX->SetLoop(true);
            }
        }
	}

	void MoveState::Exit()
	{
        m_SFX->Stop();
	}
}
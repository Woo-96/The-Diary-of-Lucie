#include "Lu_StatueScript.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_PlayerScript.h"
#include "Lu_Resources.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	StatueScript::StatueScript()
		: m_StatueColor(eStatueColor::None)
		, m_StatueType(eStatueType::None)
		, m_StatueState(eStatueState::Active)
		, m_AnotherStatue(nullptr)
		, m_Animator(nullptr)
	{
		SetName(L"StatueScript");
	}

	StatueScript::~StatueScript()
	{
	}

	void StatueScript::Initialize()
	{
	}

	void StatueScript::Update()
	{
	}
	
	void StatueScript::OnCollisionEnter(Collider2D* _Other)
	{
		if (m_StatueState == eStatueState::DeActive)
			return;

		if (_Other->GetOwner()->GetLayerIndex() == (int)eLayerType::PlayerProjectile)
		{
			StatueEfficacy();
		}
	}

	void StatueScript::CreateStatueAnimation(Animator* _Animator)
	{
		if (eStatueColor::None == m_StatueColor
			|| eStatueType::None == m_StatueType
			|| nullptr == _Animator)
			return;

		m_Animator = _Animator;

		if (eStatueColor::Red == m_StatueColor)
		{
			std::shared_ptr<Texture> pAtlas = Resources::Load<Texture>(L"RedStatue_Tex", L"..\\Resources\\Texture\\Map\\Stage\\RedStatue.png");

			if (eStatueType::Angel == m_StatueType)
			{
				m_DeActiveAnimName = L"RedAngelDeActive";
				m_Animator->Create(L"RedAngelActive", pAtlas, Vector2(0.f, 0.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.3f);
				m_Animator->Create(m_DeActiveAnimName, pAtlas, Vector2(0.f, 96.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.1f);
				m_Animator->PlayAnimation(L"RedAngelActive", true);
			}
			else
			{
				m_DeActiveAnimName = L"RedDevilDeActive";
				m_Animator->Create(L"RedDevilActive", pAtlas, Vector2(0.f, 192.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.3f);
				m_Animator->Create(m_DeActiveAnimName, pAtlas, Vector2(0.f, 288.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.1f);
				m_Animator->PlayAnimation(L"RedDevilActive", true);
			}
		}
		else 
		{
			std::shared_ptr<Texture> pAtlas = Resources::Load<Texture>(L"BlueStatue_Tex", L"..\\Resources\\Texture\\Map\\Stage\\BlueStatue.png");

			if (eStatueType::Angel == m_StatueType)
			{
				m_DeActiveAnimName = L"BlueAngelDeActive";
				m_Animator->Create(L"BlueAngelActive", pAtlas, Vector2(0.f, 0.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.3f);
				m_Animator->Create(m_DeActiveAnimName, pAtlas, Vector2(0.f, 96.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.1f);
				m_Animator->PlayAnimation(L"BlueAngelActive", true);
			}
			else
			{
				m_DeActiveAnimName = L"BlueDevilDeActive";
				m_Animator->Create(L"BlueDevilActive", pAtlas, Vector2(0.f, 192.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.3f);
				m_Animator->Create(m_DeActiveAnimName, pAtlas, Vector2(0.f, 288.f), Vector2(96.f, 96.f), 3, Vector2(96.f, 96.f), Vector2::Zero, 0.1f);
				m_Animator->PlayAnimation(L"BlueDevilActive", true);
			}
		}
	}

	void StatueScript::DeActiveStatue()
	{
		// �ڽ� ��Ȱ��ȭ
		m_StatueState = eStatueState::DeActive;
		m_Animator->PlayAnimation(m_DeActiveAnimName, false);

		// �ٸ� ���� ��Ȱ��ȭ
		if (m_AnotherStatue 
			&& m_AnotherStatue->GetStatueState() == eStatueState::Active)
		{
			m_AnotherStatue->DeActiveStatue();
		}
	}

	void StatueScript::StatueEfficacy()
	{
		DeActiveStatue();

		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

		if (m_StatueColor == eStatueColor::Red)
		{
			if (m_StatueType == eStatueType::Angel)
			{
				// ���� õ�� : ü�� 2 ȸ��
				pPlayerScript->InflictDamage(-2);
			}
			else
			{
				// ���� �Ǹ� : ü�� 4 ȸ�� or ü�� 2 ����
				int iRand = std::rand() % 2;
				if (iRand == 0)
				{
					pPlayerScript->InflictDamage(-4);
				}
				else
				{
					pPlayerScript->InflictDamage(2);
				}
			}

			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
			pSFX->SetClip(Resources::Load<AudioClip>(L"RedStatueSFX", L"..\\Resources\\Sound\\SFX\\Player\\RedStatueSFX.ogg"));
			pSFX->Play();
		}
		else
		{
			if (m_StatueType == eStatueType::Angel)
			{
				// �Ķ� õ�� : ���� �� �ִ� ���� 1 ȸ��
				pPlayerScript->GetMaxMana(1);
				pPlayerScript->UseMana(-1);
			}
			else
			{
				// �Ķ� �Ǹ� : ���� �� �ִ� ���� 2 ȸ�� or �׳� ������ 1 ����
				int iRand = std::rand() % 2;
				if (iRand == 0)
				{
					pPlayerScript->GetMaxMana(2);
					pPlayerScript->UseMana(-2);
				}
				else
				{
					pPlayerScript->UseMana(1);
				}
			}

			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
			pSFX->SetClip(Resources::Load<AudioClip>(L"BlueStatueSFX", L"..\\Resources\\Sound\\SFX\\Player\\BlueStatueSFX.ogg"));
			pSFX->Play();
		}
	}
}
#include "Lu_FireStrikeScript.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_SceneManager.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	FireStrikeScript::FireStrikeScript()
		: m_Animator(nullptr)
	{
		SetName(L"FireStrikeScript");
		SetSkillType(eSkillType::FireStrike);
	}

	FireStrikeScript::~FireStrikeScript()
	{
	}

	void FireStrikeScript::Initialize()
	{
		// 局聪皋捞记 积己
		m_Animator = GetOwner()->GetComponent<Animator>();
		if (m_Animator)
		{
			m_Animator->Create(L"FireStrike", Resources::Load<Texture>(L"FireStrike_Tex", L"..\\Resources\\Texture\\Player\\FireStrike.png")
				, Vector2::Zero, Vector2(192.f, 192.f), 14, Vector2(192.f, 192.f), Vector2::Zero, 0.1f);
			m_Animator->CompleteEvent(L"FireStrike") = std::bind(&FireStrikeScript::CompleteAnimation, this);
			m_Animator->PlayAnimation(L"FireStrike", true);
		}
	}

	void FireStrikeScript::CompleteAnimation()
	{
		object::Destroy(GetOwner());
	}
}
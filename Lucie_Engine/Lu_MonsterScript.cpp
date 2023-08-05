#include "Lu_MonsterScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"

namespace Lu
{
	MonsterScript::MonsterScript()
		: m_Animator(nullptr)
		, m_MonsterInfo{}
		, m_AnimDir(eAnimDir::Down)
		, m_PrevAnimDir(eAnimDir::Down)
	{
		SetName(L"MonsterScript");
	}

	MonsterScript::~MonsterScript()
	{

	}

	void MonsterScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		CreateAnimation();
	}
}
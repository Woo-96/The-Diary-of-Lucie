#pragma once
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class SlimeTraceState : public SlimeStateScript
	{
	public:
		SlimeTraceState();
		virtual ~SlimeTraceState();

	private:
		bool	m_bAttackType;		// 2가지 공격을 번갈아 사용하기 위한 변수
		bool	m_bAttack;			 
		float	m_AttackCoolTime;	

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}
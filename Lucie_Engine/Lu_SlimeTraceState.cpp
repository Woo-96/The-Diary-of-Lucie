#include "Lu_SlimeTraceState.h"
#include "Lu_Time.h"
#include "Lu_SlimeScript.h"

namespace Lu
{
	SlimeTraceState::SlimeTraceState()
		: m_bAttackType(true)
		, m_bAttack(false)
		, m_AttackCoolTime(0.f)
	{
		SetName(L"SlimeTraceStateScript");
		SetStateType(eState::Trace);
	}
	SlimeTraceState::~SlimeTraceState()
	{
	}

	void SlimeTraceState::Update()
	{
		Vector3 vDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vDir.Normalize();
		SetDir(vDir);
		DetermineAnimDir(vDir);
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;
		vPos += GetDir() * GetSlimeScript()->GetInfo().MoveSpeed * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);

		// 추격 범위를 벗어남
		if (CalDirToPlayer().Length() > GetSlimeScript()->GetInfo().DetectRange)
		{
			GetSlimeScript()->ChangeState(eState::Idle);
		}

		// 공격 범위에 들어옴
		if (CalDirToPlayer().Length() < GetSlimeScript()->GetInfo().AttackRange)
		{
			// 쿨타임 체크
			if (m_bAttack)
			{
				m_AttackCoolTime += (float)Time::DeltaTime();

				if (m_AttackCoolTime >= 3.f)
				{
					m_bAttack = false;
					m_AttackCoolTime = 0.f;
				}
			}
			else
			{
				m_bAttack = true;

				if (m_bAttackType)
				{
					GetSlimeScript()->ChangeState(eState::Attack);
					m_bAttackType = false;
				}
				else
				{
					GetSlimeScript()->ChangeState(eState::JumpAttack);
					m_bAttackType = true;
				}
			}
		}
	}

	void SlimeTraceState::Enter()
	{

	}

	void SlimeTraceState::Exit()
	{
		SetTime(0.f);
	}
}
#include "Lu_EntStateScript.h"
#include "Lu_EntScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	EntStateScript::EntStateScript()
		: m_StateType(eState::End)
		, m_EntScript(nullptr)
		, m_Transform(nullptr)
		, m_Target(nullptr)
		, m_ActionDir(Vector3::Zero)
		, m_Time(0.f)
	{
		SetName(L"EntStateScript");
	}

	EntStateScript::~EntStateScript()
	{
	}

	PlayerScript* EntStateScript::GetTarget()
	{
		return m_EntScript->GetTarget();
	}

	void EntStateScript::Initialize()
	{
		m_Transform = GetEntScript()->GetOwner()->GetComponent<Transform>();
	}

	void EntStateScript::RandomAttack(float _CoolTime)
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= _CoolTime)
		{
			m_Time = 0.f;

			int iRand = -1;
			if (GetEntScript()->IsRandomAttack())
				iRand = std::rand() % 5;
			else
				iRand = GetEntScript()->GetAttackNumber();

			switch ((EntScript::eAttackType)3/*(EntScript::eAttackType)iRand*/)
			{
			case EntScript::eAttackType::Thorn:
				m_EntScript->ChangeState(eState::Attack_Thorn);
				break;
			case EntScript::eAttackType::PoisonBreath:
				m_EntScript->ChangeState(eState::Attack_PoisonBreath);
				break;
			case EntScript::eAttackType::WindBreath:
				m_EntScript->ChangeState(eState::Attack_WindBreath);
				break;
			case EntScript::eAttackType::TomatoBoom:
				m_EntScript->ChangeState(eState::Attack_TomatoBoom);
				break;
			case EntScript::eAttackType::Crater:
				m_EntScript->ChangeState(eState::Attack_Crater);
				break;
			default:
				break;
			}
		}
	}

	Vector2 EntStateScript::CalDirToPlayer()
	{
		Transform* pPlayerTr = GetTarget()->GetOwner()->GetComponent<Transform>();
		Vector3 vPlayerPos = pPlayerTr->GetPosition();
		Vector3 vMonsterPos = GetTransform()->GetPosition();

		return Vector2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
	}
}
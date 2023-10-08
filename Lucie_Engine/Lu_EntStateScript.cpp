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

	void EntStateScript::ChangeStateAfterTime(float _Time, eState _State)
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= _Time)
		{
			m_Time = 0.f;
			m_EntScript->ChangeState(_State);
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
#include "Lu_SnabyStateScript.h"
#include "Lu_SnabyScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	SnabyStateScript::SnabyStateScript()
		: m_StateType(eState::End)
		, m_SnabyScript(nullptr)
		, m_Transform(nullptr)
		, m_Target(nullptr)
		, m_ActionDir(Vector3::Zero)
		, m_Time(0.f)
	{
		SetName(L"SnabyStateScript");
	}

	SnabyStateScript::~SnabyStateScript()
	{
	}

	PlayerScript* SnabyStateScript::GetTarget()
	{
		return m_SnabyScript->GetTarget();
	}

	void SnabyStateScript::Initialize()
	{
		m_Transform = GetSnabyScript()->GetOwner()->GetComponent<Transform>();
	}

	void SnabyStateScript::DetermineAnimDir(float _Angle)
	{
		if (45 < _Angle && _Angle <= 135)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (135 < _Angle && _Angle <= 225)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else if (225 < _Angle && _Angle <= 315)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Down);
		else
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Right);
	}

	void SnabyStateScript::DetermineAnimDir(Vector3 _Dir)
	{
		float fRadian = atan2(_Dir.y, _Dir.x);
		int iDegree = math::RadianToDegree(fRadian);

		if (iDegree > -45 && iDegree <= 45)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Right);
		else if (iDegree > 45 && iDegree <= 135)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (iDegree > 135 || iDegree <= -135)
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else
			GetSnabyScript()->SetCurDir(MonsterScript::eAnimDir::Down);
	}

	void SnabyStateScript::ChangeStateAfterTime(float _Time, eState _State)
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= _Time)
		{
			m_Time = 0.f;
			m_SnabyScript->ChangeState(_State);
		}
	}

	Vector2 SnabyStateScript::CalDirToPlayer()
	{
		Transform* pPlayerTr = GetTarget()->GetOwner()->GetComponent<Transform>();
		Vector3 vPlayerPos = pPlayerTr->GetPosition();
		Vector3 vMonsterPos = GetTransform()->GetPosition();

		return Vector2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
	}
}
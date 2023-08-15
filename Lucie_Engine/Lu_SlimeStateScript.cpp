#include "Lu_SlimeStateScript.h"
#include "Lu_SlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	SlimeStateScript::SlimeStateScript()
		: m_StateType(eState::End)
		, m_SlimeScript(nullptr)
		, m_Transform(nullptr)
		, m_Target(nullptr)
		, m_ActionDir(Vector3::Zero)
		, m_Time(0.f)
	{
		SetName(L"SlimeStateScript");
	}

	SlimeStateScript::~SlimeStateScript()
	{
	}

	PlayerScript* SlimeStateScript::GetTarget()
	{
		return m_SlimeScript->GetTarget();
	}

	void SlimeStateScript::Initialize()
	{
		m_Transform = GetSlimeScript()->GetOwner()->GetComponent<Transform>();
	}

	void SlimeStateScript::DetermineAnimDir(float _Angle)
	{
		if (45 < _Angle && _Angle <= 135)
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (135 < _Angle && _Angle <= 225)
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else if (225 < _Angle && _Angle <= 315)
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Down);
		else
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Right);
	}

	void SlimeStateScript::DetermineAnimDir(Vector3 _Dir)
	{
		float fRadian = atan2(_Dir.y, _Dir.x);
		int iDegree = (int)(math::RadianToDegree(fRadian));

		if (iDegree > -45 && iDegree <= 45)
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Right);
		else if (iDegree > 45 && iDegree <= 135)
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (iDegree > 135 || iDegree <= -135)
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else
			GetSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Down);
	}

	void SlimeStateScript::ChangeStateAfterTime(float _Time, eState _State)
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= _Time)
		{
			m_Time = 0.f;
			m_SlimeScript->ChangeState(_State);
		}
	}

	Vector2 SlimeStateScript::CalDirToPlayer()
	{
		Transform* pPlayerTr = GetTarget()->GetOwner()->GetComponent<Transform>();
		Vector3 vPlayerPos = pPlayerTr->GetPosition();
		Vector3 vMonsterPos = GetTransform()->GetPosition();

		return Vector2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
	}
}
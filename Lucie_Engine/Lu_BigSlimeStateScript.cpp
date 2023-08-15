#include "Lu_BigSlimeStateScript.h"
#include "Lu_BigSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	BigSlimeStateScript::BigSlimeStateScript()
		: m_StateType(eState::End)
		, m_BigSlimeScript(nullptr)
		, m_Transform(nullptr)
		, m_Target(nullptr)
		, m_ActionDir(Vector3::Zero)
		, m_Time(0.f)
	{
		SetName(L"BigSlimeStateScript");
	}

	BigSlimeStateScript::~BigSlimeStateScript()
	{
	}

	PlayerScript* BigSlimeStateScript::GetTarget()
	{
		return m_BigSlimeScript->GetTarget();
	}

	void BigSlimeStateScript::Initialize()
	{
		m_Transform = GetBigSlimeScript()->GetOwner()->GetComponent<Transform>();
	}

	void BigSlimeStateScript::DetermineAnimDir(float _Angle)
	{
		if (45 < _Angle && _Angle <= 135)
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (135 < _Angle && _Angle <= 225)
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else if (225 < _Angle && _Angle <= 315)
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Down);
		else
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Right);
	}

	void BigSlimeStateScript::DetermineAnimDir(Vector3 _Dir)
	{
		float fRadian = atan2(_Dir.y, _Dir.x);
		int iDegree = (int)(math::RadianToDegree(fRadian));

		if (iDegree > -45 && iDegree <= 45)
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Right);
		else if (iDegree > 45 && iDegree <= 135)
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (iDegree > 135 || iDegree <= -135)
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else
			GetBigSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Down);
	}

	void BigSlimeStateScript::ChangeStateAfterTime(float _Time, eState _State)
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= _Time)
		{
			m_Time = 0.f;
			m_BigSlimeScript->ChangeState(_State);
		}
	}

	Vector2 BigSlimeStateScript::CalDirToPlayer()
	{
		Transform* pPlayerTr = GetTarget()->GetOwner()->GetComponent<Transform>();
		Vector3 vPlayerPos = pPlayerTr->GetPosition();
		Vector3 vMonsterPos = GetTransform()->GetPosition();

		return Vector2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
	}
}
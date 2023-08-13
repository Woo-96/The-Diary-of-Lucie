#include "Lu_KingSlimeStateScript.h"
#include "Lu_KingSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	KingSlimeStateScript::KingSlimeStateScript()
		: m_StateType(eState::End)
		, m_KingSlimeScript(nullptr)
		, m_Transform(nullptr)
		, m_Target(nullptr)
		, m_ActionDir(Vector3::Zero)
		, m_Time(0.f)
	{
		SetName(L"KingSlimeStateScript");
	}

	KingSlimeStateScript::~KingSlimeStateScript()
	{
	}

	PlayerScript* KingSlimeStateScript::GetTarget()
	{
		return m_KingSlimeScript->GetTarget();
	}

	void KingSlimeStateScript::Initialize()
	{
		m_Transform = GetKingSlimeScript()->GetOwner()->GetComponent<Transform>();
	}

	void KingSlimeStateScript::DetermineAnimDir(float _Angle)
	{
		if (45 < _Angle && _Angle <= 135)
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (135 < _Angle && _Angle <= 225)
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else if (225 < _Angle && _Angle <= 315)
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Down);
		else
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Right);
	}

	//void KingSlimeStateScript::DetermineAnimDir(Vector3 _Dir)
	//{
	//	float fRadian = atan2(_Dir.y, _Dir.x);
	//	int iDegree = (int)(math::RadianToDegree(fRadian));

	//	if (iDegree > -45 && iDegree <= 45)
	//		GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Right);
	//	else if (iDegree > 45 && iDegree <= 135)
	//		GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Up);
	//	else if (iDegree > 135 || iDegree <= -135)
	//		GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Left);
	//	else
	//		GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Down);
	//}

	void KingSlimeStateScript::DetermineAnimDir(Vector3 _Dir)
	{
		float fRadian = atan2(_Dir.y, _Dir.x);
		int iDegree = (int)(math::RadianToDegree(fRadian));

		// 음수 각도에 대한 보정
		if (iDegree < 0)
			iDegree += 360;

		if (iDegree > 45 && iDegree <= 135)
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Up);
		else if (iDegree > 135 && iDegree <= 225)
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Left);
		else if (iDegree > 225 && iDegree <= 315)
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Down);
		else
			GetKingSlimeScript()->SetCurDir(MonsterScript::eAnimDir::Right);
	}


	void KingSlimeStateScript::ChangeStateAfterTime(float _Time, eState _State)
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= _Time)
		{
			m_Time = 0.f;
			m_KingSlimeScript->ChangeState(_State);
		}
	}

	Vector2 KingSlimeStateScript::CalDirToPlayer()
	{
		Transform* pPlayerTr = GetTarget()->GetOwner()->GetComponent<Transform>();
		Vector3 vPlayerPos = pPlayerTr->GetPosition();
		Vector3 vMonsterPos = GetTransform()->GetPosition();

		return Vector2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
	}
}
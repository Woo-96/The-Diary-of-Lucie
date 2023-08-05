#include "Lu_SnabyStateScript.h"
#include "Lu_SnabyScript.h"
#include "Lu_GameObject.h"

Lu::SnabyStateScript::SnabyStateScript()
	: m_StateType(eState::End)
	, m_SnabyScript(nullptr)
	, m_Transform(nullptr)
	, m_Target(nullptr)
	, m_ActionDir(Vector3::Zero)
	, m_Time(0.f)
{
	SetName(L"SnabyStateScript");
}

Lu::SnabyStateScript::~SnabyStateScript()
{
}

void Lu::SnabyStateScript::Initialize()
{
	m_Transform = GetSnabyScript()->GetOwner()->GetComponent<Transform>();
}

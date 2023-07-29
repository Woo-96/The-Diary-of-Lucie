#include "Lu_SnabyStateScript.h"
#include "Lu_SnabyScript.h"
#include "Lu_GameObject.h"

Lu::SnabyStateScript::SnabyStateScript()
	: m_StateType(eState::End)
	, m_SnabyScript(nullptr)
	, m_Transform(nullptr)
	, m_Target(nullptr)
	, m_ActionSpeed(300.f)
	, m_ActionDir(Vector3::Zero)
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

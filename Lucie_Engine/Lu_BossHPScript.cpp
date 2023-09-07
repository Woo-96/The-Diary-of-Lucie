#include "Lu_BossHPScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_GameObject.h"
#include "Lu_FontWrapper.h"

namespace Lu
{
	BossHPScript::BossHPScript()
		: m_MeshRender(nullptr)
		, m_MaxHP(0.f)
		, m_CurHP(0.f)
	{
		SetName(L"BossHPScript");
	}

	BossHPScript::~BossHPScript()
	{
	}

	void BossHPScript::SetCurHP(int _HP)
	{
		m_CurHP = (float)_HP;
		float HPPercent = (float)(m_CurHP / m_MaxHP);

		m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_0, &HPPercent);
	}
}
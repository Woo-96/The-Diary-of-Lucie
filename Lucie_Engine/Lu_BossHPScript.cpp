#include "Lu_BossHPScript.h"
#include "Lu_MeshRenderer.h"

namespace Lu
{
	BossHPScript::BossHPScript()
		: m_Transform(nullptr)
		, m_MeshRender(nullptr)
		, m_MaxHP(0)
		, m_CurHP(0)
	{
		SetName(L"BossHPScript");
	}

	BossHPScript::~BossHPScript()
	{
	}

	//void BossHPScript::SetCurHP(int _HP)
	//{
	//	// UV 계산
	//	m_CurHP = _HP;
	//	float HPPercentage = (float)m_CurHP / m_MaxHP;
	//	float UV = 1.0f - HPPercentage;

	//	m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_0, &UV);

	//	// 크기 계산
	//	Vector3 vScale = m_Transform->GetScale();
	//	m_Transform->SetScale(Vector3(vScale.x * HPPercentage, vScale.y , vScale.z));

	//	// LT 맞추기


	//}

	void BossHPScript::SetCurHP(int _HP)
	{
		// 텍스쳐 UV 적용
		m_CurHP = _HP;

		// HP Bar 크기 적용


		// HP Bar 위치 적용

	}
}
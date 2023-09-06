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
	//	// UV ���
	//	m_CurHP = _HP;
	//	float HPPercentage = (float)m_CurHP / m_MaxHP;
	//	float UV = 1.0f - HPPercentage;

	//	m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_0, &UV);

	//	// ũ�� ���
	//	Vector3 vScale = m_Transform->GetScale();
	//	m_Transform->SetScale(Vector3(vScale.x * HPPercentage, vScale.y , vScale.z));

	//	// LT ���߱�


	//}

	void BossHPScript::SetCurHP(int _HP)
	{
		// �ؽ��� UV ����
		m_CurHP = _HP;

		// HP Bar ũ�� ����


		// HP Bar ��ġ ����

	}
}
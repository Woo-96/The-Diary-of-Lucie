#include "Lu_LabelScript.h"
#include "Lu_GameObject.h"
#include "Lu_Time.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Object.h"

namespace Lu
{
	LabelScript::LabelScript()
		: m_MeshRender(nullptr)
        , m_Time(0.f)
        , m_Duration(0.f)
        , m_bBlink(false)
	{
		SetName(L"LabelScript");
	}

	LabelScript::~LabelScript()
	{
	}

    void LabelScript::Update()
    {
        if (GetOwner()->IsDead() || nullptr == m_MeshRender)
            return;

        if (m_bBlink)
        {
            m_Time += (float)Time::DeltaTime();

            const float blinkPeriod = 2.f;
            float fAlpha = fabs(sin(2.f * PI * m_Time / blinkPeriod));
            m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::INT_3, &m_bBlink);
            m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_3, &fAlpha);
        }
        else
        {
            m_Time += (float)Time::DeltaTime();

            float fAlpha;

            if (m_Time <= m_Duration / 2.0f)
            {
                // ó�� 1.5�� ���� ���� �� ����
                fAlpha = m_Time / (m_Duration / 2.0f);
            }
            else
            {
                // ������ 1.5�� ���� ���� �� ����
                fAlpha = 1.0f - (m_Time - m_Duration / 2.0f) / (m_Duration / 2.0f);
            }

            // Alpha ���� 0���� ������ ������Ʈ �ı�
            if (fAlpha <= 0.0f)
            {
                object::Destroy(GetOwner());
            }

            int bFading = true;
            m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::INT_3, &bFading);
            m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_3, &fAlpha);
        }
    }

    void LabelScript::SetActive(bool _b)
    {
        GetOwner()->SetActive(_b);
    }
}

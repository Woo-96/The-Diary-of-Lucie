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
	{
		SetName(L"LabelScript");
	}

	LabelScript::~LabelScript()
	{
	}

    void LabelScript::Update()
    {
        if (GetOwner()->IsDead() || nullptr == m_MeshRender || 0.f == m_Duration)
            return;

        m_Time += (float)Time::DeltaTime();

        float fAlpha;

        if (m_Time <= m_Duration / 2.0f)
        {
            // 처음 1.5초 동안 알파 값 증가
            fAlpha = m_Time / (m_Duration / 2.0f);
        }
        else
        {
            // 나머지 1.5초 동안 알파 값 감소
            fAlpha = 1.0f - (m_Time - m_Duration / 2.0f) / (m_Duration / 2.0f);
        }

        // Alpha 값이 0보다 작으면 오브젝트 파괴
        if (fAlpha <= 0.0f)
        {
            object::Destroy(GetOwner());
        }

        int bFading = true;
        m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::INT_3, &bFading);
        m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_3, &fAlpha);
    }
}

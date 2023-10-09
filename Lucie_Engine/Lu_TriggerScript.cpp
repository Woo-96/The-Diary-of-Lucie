#include "Lu_TriggerScript.h"
#include "Lu_Object.h"

namespace Lu
{
	TriggerScript::TriggerScript()
		: m_Inst(nullptr)
		, m_Delegate(nullptr)
	{
		SetName(L"TriggerScript");
	}

	TriggerScript::~TriggerScript()
	{
	}

	void TriggerScript::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::Player == _Other->GetOwner()->GetLayerIndex())
		{
			if (m_Inst && m_Delegate)
			{
				(m_Inst->*m_Delegate)();
			}

			object::Destroy(GetOwner());
		}
	}
}
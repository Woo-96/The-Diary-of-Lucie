#include "Lu_WoodScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	WoodScript::WoodScript()
		: m_Player(nullptr)
	{
	}

	WoodScript::~WoodScript()
	{
	}

	void WoodScript::OnCollisionStay(Collider2D* _Other)
	{
		if ((int)eLayerType::Player == _Other->GetOwner()->GetLayerIndex())
		{
			if (m_Player)
				m_Player->SetWood(true);
		}
	}
}

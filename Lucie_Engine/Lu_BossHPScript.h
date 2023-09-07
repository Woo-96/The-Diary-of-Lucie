#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class MeshRenderer;
	class BossHPScript : public UIScript
	{
	public:
		BossHPScript();
		virtual ~BossHPScript();

	private:
		MeshRenderer*	m_MeshRender;
		float			m_MaxHP;
		float			m_CurHP;

	public:
		void SetMeshRender(MeshRenderer* _MeshRender)
		{
			m_MeshRender = _MeshRender;
		}

		void SetMaxHP(int _MaxHP)
		{
			m_MaxHP = _MaxHP;
		}

	public:
		void SetCurHP(int _HP);
	};
}

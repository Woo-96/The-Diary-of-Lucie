#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class Transform;
	class MeshRenderer;
	class BossHPScript : public UIScript
	{
	public:
		BossHPScript();
		virtual ~BossHPScript();

	private:
		Transform*		m_Transform;
		MeshRenderer*	m_MeshRender;
		int				m_MaxHP;
		int				m_CurHP;

	public:
		void SetTransform(Transform* _Transform)
		{
			m_Transform = _Transform;
		}

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

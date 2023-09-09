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
		float			m_MaxHP;
		float			m_CurHP;
		std::wstring	m_BossName;

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
			m_MaxHP = (float)_MaxHP;
		}

		void SetBossName(std::wstring _BossName)
		{
			m_BossName = _BossName;
		}

	public:
		void SetCurHP(int _HP);

	public:
		virtual void Render() override;
	};
}

#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class Transform;
	class MeshRenderer;
	class ProgressBarScript : public UIScript
	{
	public:
		ProgressBarScript();
		virtual ~ProgressBarScript();

	private:
		Transform*		m_Transform;
		MeshRenderer*	m_MeshRender;
		float			m_MaxValue;
		float			m_CurValue;
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

		void SetMaxValue(int _MaxValue)
		{
			m_MaxValue = (float)_MaxValue;
		}

		void SetBossName(std::wstring _BossName)
		{
			m_BossName = _BossName;
		}

	public:
		void SetCurValue(int _Value);
		void SetCurValue(float _Value);

	public:
		virtual void Update() override {}
	//	virtual void Render() override;
	};
}

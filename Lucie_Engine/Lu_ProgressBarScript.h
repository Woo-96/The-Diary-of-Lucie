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
		bool			m_bTextPrint;
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

		void SetTextPrint(bool _b)
		{
			m_bTextPrint = _b;
		}

	public:
		void SetCurValue(int _Value);
		void SetCurValue(float _Value);
		void SetValuePercent(float _ValuePercent);

	public:
		virtual void Update() override {}
		virtual void Render() override;

	public:
		virtual void SetActive(bool _b) override;
	};
}

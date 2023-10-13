#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class MeshRenderer;
	class LabelScript : public UIScript
	{
	public:
		LabelScript();
		virtual ~LabelScript();

	private:
		MeshRenderer*	m_MeshRender;
		float			m_Time;
		float			m_Duration;
		bool			m_bBlink;

	public:
		void SetMeshRender(MeshRenderer* _MeshRender)
		{
			m_MeshRender = _MeshRender;
		}

		void SetDuration(float _Duration)
		{
			m_Duration = _Duration;
		}

		void SetBlink(bool _b)
		{
			m_bBlink = _b;
		}

	public:
		virtual void Update() override;
		virtual void Render() override {}

	public:
		virtual void SetActive(bool _b) override;
	};
}

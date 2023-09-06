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

	public:
		void SetMeshRender(MeshRenderer* _MeshRender)
		{
			m_MeshRender = _MeshRender;
		}

		void SetDuration(float _Duration)
		{
			m_Duration = _Duration;
		}

	public:
		virtual void Update() override;
	};
}

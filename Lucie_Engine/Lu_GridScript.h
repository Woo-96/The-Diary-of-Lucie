#pragma once
//#include "Lu_Script.h"
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "..\\Engine_SOURCE\\Lu_Camera.h"

namespace Lu
{
	class GridScript : public Script
	{
	public:
		GridScript();
		virtual ~GridScript();

	private:
		Camera*		m_Camera;

	public:
		void SetCamera(Camera* _Camera)
		{ 
			m_Camera = _Camera; 
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}

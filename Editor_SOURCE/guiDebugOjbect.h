#pragma once
#include "..\\Engine_SOURCE\\Lu_GameObject.h"


namespace gui
{
	class DebugOjbect : public Lu::GameObject
	{
	public:
		DebugOjbect();
		virtual ~DebugOjbect();

	private:

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}

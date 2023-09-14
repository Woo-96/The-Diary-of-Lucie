#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class UIScript : public Script
	{
	public:
		UIScript();
		virtual ~UIScript();

	public:
		virtual void Initialize() override {}
		virtual void Update() override {};
		virtual void LateUpdate() override {}
		virtual void Render() override {}
	};
}
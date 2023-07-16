#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class Gold : public UIScript
	{
	public:
		Gold();
		virtual ~Gold();

	public:
		virtual void Update() override;
	};
}
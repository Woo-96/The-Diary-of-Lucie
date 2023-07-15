#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class HP : public UIScript
	{
	public:
		HP();
		virtual ~HP();

	public:
		virtual void Update() override;
	};
}
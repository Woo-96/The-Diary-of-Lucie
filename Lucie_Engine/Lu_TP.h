#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class TP : public UIScript
	{
	public:
		TP();
		virtual ~TP();

	public:
		virtual void Update() override;
	};
}
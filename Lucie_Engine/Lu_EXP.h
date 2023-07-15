#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class EXP : public UIScript
	{
	public:
		EXP();
		virtual ~EXP();

	public:
		virtual void Update() override;
	};
}
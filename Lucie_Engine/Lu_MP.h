#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class MP : public UIScript
	{
	public:
		MP();
		virtual ~MP();

	public:
		virtual void Update() override;
	};
}
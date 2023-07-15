#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class Layout : public UIScript
	{
	public:
		Layout();
		virtual ~Layout();

	public:
		virtual void Update() override;
	};
}

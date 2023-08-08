#pragma once
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class KingSlimeTraceState : public KingSlimeStateScript
	{
	public:
		KingSlimeTraceState();
		virtual ~KingSlimeTraceState();

	public:
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}

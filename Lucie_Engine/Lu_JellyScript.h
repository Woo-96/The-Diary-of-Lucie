#pragma once
#include "Lu_ItemScript.h"

namespace Lu
{
	class JellyScript :	public ItemScript
	{
	public:
		JellyScript();
		virtual ~JellyScript();

	public:
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;

	public:
		virtual bool ItemEfficacy() override;
	};
}

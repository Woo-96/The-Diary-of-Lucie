#pragma once
#include "Lu_Component.h"
#include "Lu_Collider2D.h"

namespace Lu
{
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

	public:
		virtual void Initialize() override {};
		virtual void Update() override {};
		virtual void LateUpdate() override {};
		virtual void Render() override {};

	public:
		virtual void OnCollisionEnter(Collider2D* other) {};
		virtual void OnCollisionStay(Collider2D* other) {};
		virtual void OnCollisionExit(Collider2D* other) {};
	};
}
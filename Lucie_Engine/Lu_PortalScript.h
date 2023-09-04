#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
    class PortalScript : public Script
    {
    public:
        PortalScript();
        virtual ~PortalScript();

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
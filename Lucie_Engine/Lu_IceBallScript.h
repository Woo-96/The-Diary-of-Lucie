#pragma once
#include "Lu_ProjectileScript.h"

namespace Lu
{
	class IceBallScript : public ProjectileScript
	{
    public:
        IceBallScript();
        virtual ~IceBallScript();

    public:
        virtual void Update() override;

    public:
        virtual void OnCollisionEnter(Collider2D* _Other) override;

    public:
        virtual int GetProjectileDamage() override;
	};
}


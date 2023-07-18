#pragma once
#include "Lu_StageScene.h"

namespace Lu
{
	class Nomal1Scene : public StageScene
	{
	public:
		Nomal1Scene();
		virtual ~Nomal1Scene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Destroy() override;

	public:
		virtual void OnEnter() override;
		virtual void OnExit() override;
	};
}
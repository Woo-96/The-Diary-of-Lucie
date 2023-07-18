#pragma once
#include "Lu_StageScene.h"

namespace Lu
{
	class BossWayScene : public StageScene
	{
	public:
		BossWayScene();
		virtual ~BossWayScene();

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
#pragma once
#include "Lu_StageScene.h"

namespace Lu
{
	class StoreScene : public StageScene
	{
	public:
		StoreScene();
		virtual ~StoreScene();

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
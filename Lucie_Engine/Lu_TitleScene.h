#pragma once
#include "..\Engine_SOURCE\Lu_Scene.h"

namespace Lu
{
	class TitleScene : public Scene
	{
	public:
		TitleScene();
		virtual ~TitleScene();

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
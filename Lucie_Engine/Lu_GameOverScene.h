#pragma once
//#include <Lu_Scene.h>
#include "..\Engine_SOURCE\Lu_Scene.h"

namespace Lu
{
	class GameOverScene : public Scene
	{
	public:
		GameOverScene();
		virtual ~GameOverScene();

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
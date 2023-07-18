#pragma once
#include "Lu_StageScene.h"

namespace Lu
{
	class Nomal2Scene : public StageScene
	{
	public:
		Nomal2Scene();
		virtual ~Nomal2Scene();

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
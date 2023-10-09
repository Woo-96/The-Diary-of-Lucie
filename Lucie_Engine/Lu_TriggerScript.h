#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class Scene;
	class TriggerScript : public Script
	{
	public:
		typedef void (Scene::* Scene_DELEGATE)(void);

	public:
		TriggerScript();
		virtual ~TriggerScript();

	private:
		Scene*			m_Inst;
		Scene_DELEGATE	m_Delegate;

	public:
		void AddDelegate(Scene* _Inst, Scene_DELEGATE _Func)
		{
			m_Inst = _Inst;
			m_Delegate = _Func;
		}

	public:
		virtual void OnCollisionEnter(Collider2D* _Other);
	};
}

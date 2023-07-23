#pragma once
#include "Lu_Component.h"
#include "Lu_Animation.h"

namespace Lu
{
	class Animator : public Component
	{
	public:
		struct Event
		{
			void operator=(std::function<void()> _Func)
			{
				m_Event = std::move(_Func);
			}
			void operator()()
			{
				if (m_Event)
					m_Event();
			}

			std::function<void()> m_Event;
		};

		struct Events
		{
			Event startEvent;
			Event completeEvent;
			Event endEvent;
		};

	public:
		Animator();
		virtual ~Animator();

	private:
		std::map<std::wstring, Animation*>	m_Animations;
		std::map<std::wstring, Events*>		m_Events;
		Animation*							m_ActiveAnimation;

		bool								m_bLoop;

	public:
		virtual void Update();

	public:
		void Create(const std::wstring& _Name
			, std::shared_ptr<graphics::Texture> _Atlas
			, Vector2 _LeftTop, Vector2 _Size, UINT _ColumnLength, Vector2 _BackSize
			, Vector2 _Offset = Vector2::Zero, float _Duration = 0.1f, bool _Reverse = false);
		Animation* FindAnimation(const std::wstring& _Name);
		Events* FindEvents(const std::wstring& _Name);
		void PlayAnimation(const std::wstring& _Name, bool _Loop);
		void Binds();

		std::function<void()>& StartEvent(const std::wstring _Key);
		std::function<void()>& CompleteEvent(const std::wstring _Key);
		std::function<void()>& EndEvent(const std::wstring _Key);
	};
}



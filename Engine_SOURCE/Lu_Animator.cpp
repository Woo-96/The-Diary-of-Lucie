#include "Lu_Animator.h"

namespace Lu
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, m_ActiveAnimation(nullptr)
		, m_bLoop(true)
	{
		SetName(L"Animator");
	}

	Animator::~Animator()
	{
		for (auto& iter : m_Animations)
		{
			delete iter.second;
			iter.second = nullptr;
		}


		for (auto& iter : m_Events)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}

	void Animator::Update()
	{
		if (m_ActiveAnimation == nullptr)
			return;

		if (m_ActiveAnimation->IsComplete() && m_bLoop)
		{
			Events* events
				= FindEvents(m_ActiveAnimation->GetKey());
			if (events)
				events->completeEvent();

			m_ActiveAnimation->Reset();
		}

		m_ActiveAnimation->LateUpdate();
	}

	void Animator::Create(const std::wstring& _Name, std::shared_ptr<graphics::Texture> _Atlas, Vector2 _LeftTop, Vector2 _Size, UINT _ColumnLength
		, Vector2 _BackSize, Vector2 _Offset, float _Duration, bool _Reverse)
	{
		Animation* animation = FindAnimation(_Name);
		if (nullptr != animation)
			return;

		animation = new Animation();
		animation->SetKey(_Name);

		animation->Create(_Name, _Atlas, _LeftTop, _Size, _ColumnLength, _BackSize, _Offset, _Duration, _Reverse);

		m_Animations.insert(std::make_pair(_Name, animation));

		Events* events = FindEvents(_Name);
		if (events != nullptr)
			return;

		events = new Events();
		m_Events.insert(std::make_pair(_Name, events));
	}

	Animation* Animator::FindAnimation(const std::wstring& _Name)
	{
		std::map<std::wstring, Animation*>::iterator iter = m_Animations.find(_Name);

		if (iter == m_Animations.end())
			return nullptr;

		return iter->second;
	}

	Animator::Events* Animator::FindEvents(const std::wstring& _Name)
	{
		std::map<std::wstring, Events*>::iterator iter = m_Events.find(_Name);

		if (iter == m_Events.end())
			return nullptr;

		return iter->second;
	}

	void Animator::PlayAnimation(const std::wstring& _Name, bool _Loop)
	{
		Animation* animation = FindAnimation(_Name);

		if (m_ActiveAnimation == animation)
			return;

		Animation* prevAnimation = m_ActiveAnimation;

		Events* events;
		if (prevAnimation != nullptr)
		{
			events = FindEvents(prevAnimation->GetKey());
			if (events)
				events->endEvent();
		}

		if (animation)
		{
			m_ActiveAnimation = animation;
		}

		events = FindEvents(m_ActiveAnimation->GetKey());
		if (events)
			events->startEvent();

		m_bLoop = _Loop;
		m_ActiveAnimation->Reset();
	}

	void Animator::Binds()
	{
		if (m_ActiveAnimation == nullptr)
			return;

		m_ActiveAnimation->Binds();
	}

	std::function<void()>& Animator::StartEvent(const std::wstring _Key)
	{
		Events* events = FindEvents(_Key);

		return events->startEvent.m_Event;
	}

	std::function<void()>& Animator::CompleteEvent(const std::wstring _Key)
	{
		Events* events = FindEvents(_Key);

		return events->completeEvent.m_Event;
	}

	std::function<void()>& Animator::EndEvent(const std::wstring _Key)
	{
		Events* events = FindEvents(_Key);

		return events->endEvent.m_Event;
	}
}

#pragma once
#include "Lu_ItemScript.h"

namespace Lu
{
	class Animator;
	class CoinScript : public ItemScript
	{
	public:
		enum class eCoinType
		{
			None,
			Copper,
			Silver,
			Gold,
		};

	public:
		CoinScript();
		virtual ~CoinScript();

	private:
		eCoinType	m_CoinType;
		Animator*	m_Animator;

	public:
		void SetCoinType(eCoinType _CoinType)
		{
			m_CoinType = _CoinType;
		}

		void SetAnimator(Animator* _Animator)
		{
			m_Animator = _Animator;
		}

	public:
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;

	public:
		void CreateCoinAnimation();
		virtual bool ItemEfficacy() override;
	};
}
#pragma once
#include "Lu_UIScript.h"
#include "Lu_Material.h"

#define MaxMana	5

namespace Lu
{
	class GameObject;
	class ManaScript : public UIScript
	{
	public:
		enum class eManaColor
		{
			Empty,
			Full,
			End,
		};

	public:
		ManaScript();
		virtual ~ManaScript();

	private:
		GameObject*					m_arrMana[MaxMana * 2];					// 마나 객체
		std::shared_ptr<Material>	m_arrMaterial[(int)eManaColor::End];	// 마나 재질
		Vector2						m_arrManaPos[MaxMana];					// 마나 객체 위치
		int							m_MaxCount;								// 현재 플레이어 최대 마나 객체 개수

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		void SetMaxMP(int _MaxMP);
		void SetMana(int _CurMP);

	private:
		void ChangeManaColor(int _Index, eManaColor _Color);

	public:
		virtual void SetActive(bool _b) override;
	};
}
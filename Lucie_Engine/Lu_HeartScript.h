#pragma once
#include "Lu_UIScript.h"
#include "Lu_Material.h"

#define MaxHeart	5

namespace Lu
{
	class GameObject;
	class HeartScript : public UIScript
	{
	public:
		enum class eHeartColor
		{
			Empty,
			Full,
			End,
		};

		enum class eHeartSize
		{
			Small,
			Big,
			End,
		};

	public:
		HeartScript();
		virtual ~HeartScript();

	private:
		GameObject*					m_arrHeart[MaxHeart * 2];				// 하트 객체
		std::shared_ptr<Material>	m_arrMaterial[(int)eHeartColor::End];	// 하트 재질
		Vector2						m_arrHeartPos[MaxHeart];				// 하트 객체 위치
		//Vector2						m_arrHeartUV[MaxHeart * 2];				// 하트 렌더링 여부 (Slice를 0으로 설정하면 렌더링 안함)
		int							m_MaxCount;								// 현재 플레이어 최대 하트 객체 개수

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		void SetMaxHP(int _MaxHP);			// 현재 플레이어 최대 체력으로 Heart 객체 개수 계산
		void SetHeart(int _CurHP);			// 현재 플레이어 체력으로 하트 렌더링

	private:
		void ChangeHeartColor(int _Index, eHeartColor _Color);
		void ChangeHeartSize(int _Index, eHeartSize _Size);
		//void HeartRender(int _Index, bool _Render);
	};
}
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
		GameObject*					m_arrHeart[MaxHeart * 2];				// ��Ʈ ��ü
		std::shared_ptr<Material>	m_arrMaterial[(int)eHeartColor::End];	// ��Ʈ ����
		Vector2						m_arrHeartPos[MaxHeart];				// ��Ʈ ��ü ��ġ
		//Vector2						m_arrHeartUV[MaxHeart * 2];				// ��Ʈ ������ ���� (Slice�� 0���� �����ϸ� ������ ����)
		int							m_MaxCount;								// ���� �÷��̾� �ִ� ��Ʈ ��ü ����

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		void SetMaxHP(int _MaxHP);			// ���� �÷��̾� �ִ� ü������ Heart ��ü ���� ���
		void SetHeart(int _CurHP);			// ���� �÷��̾� ü������ ��Ʈ ������

	private:
		void ChangeHeartColor(int _Index, eHeartColor _Color);
		void ChangeHeartSize(int _Index, eHeartSize _Size);
		//void HeartRender(int _Index, bool _Render);
	};
}
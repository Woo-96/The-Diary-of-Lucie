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
		GameObject*					m_arrMana[MaxMana * 2];					// ���� ��ü
		std::shared_ptr<Material>	m_arrMaterial[(int)eManaColor::End];	// ���� ����
		Vector2						m_arrManaPos[MaxMana];					// ���� ��ü ��ġ
		int							m_MaxCount;								// ���� �÷��̾� �ִ� ���� ��ü ����

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
#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class Transform;
	class Camera;
	class DamageFontScript : public UIScript
	{
	public:
		DamageFontScript();
		virtual ~DamageFontScript();

	private:
		Transform*	m_Transform;
		Camera*		m_MainCam;
		int			m_Damage;
		float		m_Time;

	public:
		void SetDamage(int _Damage)
		{
			m_Damage = _Damage;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	public:
		virtual void SetActive(bool _b) override;
	};
}

#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class Transform;
	class ChannelingBarScript :	public UIScript
	{
	public:
		enum class eParts
		{
			BarFram,
			Bar,
			ChargeFX,
			FullChargeFX,
			End,
		};

		enum class eChannelingType
		{
			Charging,
			Consuming,
			End,
		};

	public:
		ChannelingBarScript();
		virtual ~ChannelingBarScript();

	private:
		Transform*		m_PlayerTransform;
		GameObject*		m_arrParts[(int)eParts::End];
		bool			m_bFirst;
		bool			m_bFXFirst;
		bool			m_bActive;
		eChannelingType	m_ChannelingType;
		float			m_Time;
		float			m_MaxTime;

	public:
		void SetPlayerTransform(Transform* _Transform)
		{
			m_PlayerTransform = _Transform;
		}

		void SetChannelingType(eChannelingType _Type);

		void SetMaxTime(float _MaxTime)
		{
			m_MaxTime = _MaxTime;
		}

	public:
		bool IsComplete();
		float GetChargeGauge();

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		void ChannelingOnOff(bool _b);

	private:
		void CompleteFX();
	};
}



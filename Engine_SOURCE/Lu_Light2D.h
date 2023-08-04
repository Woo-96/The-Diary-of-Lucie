#pragma once
#include "Lu_Component.h"
#include "Lu_Graphics.h"

namespace Lu
{
	using namespace graphics;
	class Light2D : public Component
	{
	public:
		Light2D();
		virtual ~Light2D();

	private:
		LightAttribute m_Attribute;

	public:
		void SetType(eLightType _Type) 
		{ 
			m_Attribute.Type = _Type;
		}

		void SetRadius(float _Radius) 
		{ 
			m_Attribute.Radius = _Radius; 
		}

		void SetAngle(float _Angle) 
		{ 
			m_Attribute.Angle = _Angle; 
		}

		void SetColor(Vector4 _Color) 
		{ 
			m_Attribute.Color = _Color; 
		}

	public:
		LightAttribute GetAttribute() const
		{ 
			return m_Attribute;
		}

		Vector4 GetColor() const
		{
			return m_Attribute.Color; 
		}

		eLightType GetType() const
		{ 
			return m_Attribute.Type; 
		}

		float GetRadius() const
		{ 
			return m_Attribute.Radius;
		}

		float GetAngle() const
		{
			return m_Attribute.Angle;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}

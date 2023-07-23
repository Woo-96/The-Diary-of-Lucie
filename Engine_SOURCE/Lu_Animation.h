#pragma once
#include "Lu_Resource.h"
#include "Lu_Texture.h"

namespace Lu
{
	using namespace math;
	using namespace graphics;

	class Animator;
	class Animation : public Resource
	{
	public:
		struct Sprite
		{
			Vector2 LeftTop;
			Vector2 Size;
			Vector2 Offset;
			Vector2 AtlasSize;
			float	Duration;
			bool	Reverse;

			Sprite()
				: LeftTop(Vector2::Zero)
				, Size(Vector2::Zero)
				, Offset(Vector2::Zero)
				, AtlasSize(Vector2::Zero)
				, Duration(0.0f)
				, Reverse(false)
			{

			}
		};

	public:
		Animation();
		virtual ~Animation();

	private:
		Animator*							m_Animator;
		std::shared_ptr<graphics::Texture>	m_Atlas;
		std::vector<Sprite>					m_Sprites;
		int									m_Index;
		float								m_Time;
		bool								m_bComplete;

	public:
		bool IsComplete() const
		{ 
			return m_bComplete; 
		}

	public:
		void Update();
		void LateUpdate();
		void Render();

	public:
		void Create(std::wstring _Name
			, std::shared_ptr<graphics::Texture> _Atlas
			, Vector2 _LeftTop, Vector2 _Size, UINT _ColumnLength, Vector2 _BackSize
			, Vector2 _Offset = Vector2::Zero, float _Duration = 0.0f, bool _Reverse = false);

		void Binds();
		void Reset();

	public:
		virtual HRESULT Load(const std::wstring& path) { return S_FALSE; };
	};
}


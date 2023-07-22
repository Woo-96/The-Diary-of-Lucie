#include "Lu_Animation.h"
#include "Lu_Time.h"
#include "Lu_Renderer.h"

namespace Lu
{
	Animation::Animation()
		: Resource(enums::eResourceType::Animation)
		, m_Animator(nullptr)
		, m_Index(-1)
		, m_Time(0.0f)
		, m_bComplete(false)
	{
	}

	Animation::~Animation()
	{

	}

	void Animation::Update()
	{

	}

	void Animation::LateUpdate()
	{
		if (m_bComplete)
			return;

		m_Time += (float)Time::DeltaTime();

		if (m_Sprites[m_Index].Duration <= m_Time)
		{
			m_Index++;
			m_Time = 0.0f;

			if (m_Sprites.size() <= m_Index)
			{
				m_Index = (int)m_Sprites.size() - 1;
				m_bComplete = true;
			}
		}
	}

	void Animation::Render()
	{

	}

	void Animation::Create(std::wstring _Name, std::shared_ptr<graphics::Texture> _Atlas, Vector2 _LeftTop, Vector2 _Size, UINT _ColumnLength, Vector2 _Offset, float _Duration)
	{
		SetKey(_Name);
		m_Atlas = _Atlas;


		float width = (float)_Atlas->GetWidth();
		float height = (float)_Atlas->GetHeight();

		for (size_t i = 0; i < _ColumnLength; i++)
		{
			Sprite sprite = {};
			sprite.LeftTop.x = _LeftTop.x + (i * _Size.x) / width;
			sprite.LeftTop.y = _LeftTop.y / height;
			sprite.Size.x = _Size.x / width;
			sprite.Size.y = _Size.y / height;
			sprite.Offset = _Offset;
			sprite.AtlasSize = Vector2(200.0f / width, 200.0f / height);
			sprite.Duration = _Duration;

			m_Sprites.push_back(sprite);
		}
	}

	void Animation::Binds()
	{
		m_Atlas->BindShader(graphics::eShaderStage::PS, 12);

		renderer::AnimatorCB data = {};

		data.SpriteLeftTop = m_Sprites[m_Index].LeftTop;
		data.SpriteSize = m_Sprites[m_Index].Size;
		data.SpriteOffset = m_Sprites[m_Index].Offset;
		data.AtlasSize = m_Sprites[m_Index].AtlasSize;
		data.AnimationType = 1;

		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Animator];
		cb->SetData(&data);

		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	void Animation::Reset()
	{
		m_Time = 0.0f;
		m_bComplete = false;
		m_Index = 0;
	}
}

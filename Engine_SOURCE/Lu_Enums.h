#pragma once

namespace Lu::enums
{
	enum class eComponentType
	{
		None,
		Transform,	// ��ġ ���� ������Ʈ�� ���� ���� �̷������ ��
		MeshRenderer,
		Collider2D,
		Camera,
		Animator,
		Particle,
		Script,
		End,
	};

	enum class eLayerType
	{
		Default,
		BackGround,
		Player,
		Monster,
		UI,
		Mouse,
		End = 16,
	};

	enum class eResourceType
	{
		Texture,
		Shader,
		Material,
		Mesh,
		Animation,
		End,
	};

	enum class eColliderType
	{
		Rect,
		Circle,
		Sphere,
		Cube,
		End,
	};
}
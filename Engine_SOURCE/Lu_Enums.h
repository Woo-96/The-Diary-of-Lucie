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
		Particle,
		Script,
		End,
	};

	enum class eLayerType
	{
		Default,
		Grid,
		Player,
		Monster,
		UI,
		End = 16,
	};

	enum class eResourceType
	{
		Texture,
		Shader,
		Material,
		Mesh,
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
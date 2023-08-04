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
		Light,
		Particle,
		Script,
		End,
	};

	enum class eLayerType
	{
		Default,
		BackGround,
		Immovable,
		Monster,
		Player,
		MonsterProjectile,
		PlayerProjectile,
		Light,
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
		ComputeShader,
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

	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		End,
	};
}
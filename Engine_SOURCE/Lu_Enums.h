#pragma once

namespace Lu::enums
{
	enum class eComponentType
	{
		None,
		Transform,	// 위치 정보 업데이트가 가장 먼저 이루어져야 함
		MeshRenderer,
		Collider2D,
		Camera,
		Animator,
		Light,
		Particle,
		AudioListener,
		AudioSource,
		Script,
		End,
	};

	enum class eLayerType
	{
		Default,
		BackGround,
		Ground,
		Immovable,
		Portal,
		Statue,
		Item,
		Monster,
		Player,
		MonsterProjectile,
		PlayerProjectile,
		Light,
		FX,
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
		AudioClip,
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

	enum class PARTICLE_MODULE
	{
		PARTICLE_SPAWN,
		COLOR_CHANGE,
		SCALE_CHANGE,
		ADD_VELOCITY,

		DRAG,
		NOISE_FORCE,
		RENDER,
		DUMMY_3,

		END,
	};
}
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
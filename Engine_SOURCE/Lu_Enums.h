#pragma once

namespace Lu::enums
{
	enum class eComponentType
	{
		None,
		Transform,	// 위치 정보 업데이트가 가장 먼저 이루어져야 함
		Mesh,
		Particle,
		MeshRenderer,
		Script,
		Camera,
		End,
	};

	enum class eLayerType
	{
		Player,
		Monster,
		UI,
		End,
	};

	enum class eResourceType
	{
		Texture,
		Shader,
		Material,
		Mesh,
		End,
	};
}
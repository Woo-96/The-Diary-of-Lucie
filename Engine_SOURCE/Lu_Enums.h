#pragma once

namespace Lu::enums
{
	enum class eComponentType
	{
		None,
		Transform,	// ��ġ ���� ������Ʈ�� ���� ���� �̷������ ��
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
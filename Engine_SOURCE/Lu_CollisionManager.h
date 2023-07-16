#pragma once
#include "Lucie_Engine.h"

namespace Lu
{
#define LAYER_MAX (UINT)eLayerType::End 
	using namespace enums;

	class Collider2D;
	class CollisionManager
	{
	public:
		union ColliderID
		{
			struct
			{
				UINT Left;
				UINT Right;
			};
			UINT64 id;
		};

	private:
		static std::bitset<LAYER_MAX> m_Matrix[LAYER_MAX];
		static std::map<UINT64, bool> m_CollisionMap;

	public:
		static void Initialize();
		static void Update();

	private:
		static void LayerCollision(eLayerType _Left, eLayerType _Right);
		static void ColliderCollision(Collider2D* _Left, Collider2D* _Right);
		static bool Intersect(Collider2D* _Left, Collider2D* _Right);

	public:
		static void SetLayer(eLayerType _Left, eLayerType _Right, bool _bEnable);
		static void Clear();
	};
}
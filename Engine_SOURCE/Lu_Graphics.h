#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Lu_Enums.h"
#include "Lu_Math.h"

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

#define CBSLOT_TRANSFORM		0
#define CBSLOT_Material			1
#define CBSLOT_ANIMATION2D		2

namespace Lu::graphics
{
	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		End,
	};

	enum class eCBType
	{
		Transform,
		Material,
		Animator,
		End,
	};

	enum class eSamplerType
	{
		Point,
		Anisotropic,
		End,
	};

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		End,
	};

	enum class eDSType
	{
		Less,
		Greater,
		NoWrite,
		None,
		End,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		End,
	};

	enum class eRenderingMode
	{
		Opaque,			// ºÒÅõ¸í
		CutOut,			// ¹ÝÅõ¸í (ex. ³ª¹µÀÙ, Âõ¾îÁø ¿Ê°¨)
		Transparent,	// Åõ¸í (ex. À¯¸®, ÇÃ¶ó½ºÆ½)
		End,
	};

	struct GpuBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer>	Buffer;
		D3D11_BUFFER_DESC						Desc;

		GpuBuffer()
			: Buffer(nullptr)
			, Desc{}
		{

		}
		virtual ~GpuBuffer() = default;
	};

	struct DebugMesh
	{
		enums::eColliderType	Type;
		math::Vector3			Position;
		math::Vector3			Rotation;
		math::Vector3			Scale;

		float					Radius;
		float					Duration;
		float					Time;
		math::Vector4			Color;
	};

	struct tMtrlConst
	{
		int				arrInt[4];
		float			arrFloat[4];
		math::Vector2	arrV2[4];
		math::Vector4	arrV4[4];
	};

	enum SCALAR_PARAM
	{
		INT_0,
		INT_1,
		INT_2,
		INT_3,

		FLOAT_0,
		FLOAT_1,
		FLOAT_2,
		FLOAT_3,

		VEC2_0,
		VEC2_1,
		VEC2_2,
		VEC2_3,

		VEC4_0,
		VEC4_1,
		VEC4_2,
		VEC4_3,

		SCALAR_END,
	};
}

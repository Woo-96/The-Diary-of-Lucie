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
#define CBSLOT_PARTICLE			1
#define CBSLOT_GRID				2
#define	CBSLOT_NUMBER			3
#define CBSLOT_ANIMATION2D		4

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
		Grid,
		Number,
		Animator,
		Button,
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
	};
}

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
#define CBSLOT_GLOBAL			3

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
		Global,
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
		Opaque,			// 불투명
		CutOut,			// 반투명 (ex. 나뭇잎, 찢어진 옷감)
		Transparent,	// 투명 (ex. 유리, 플라스틱)
		End,
	};

	enum class eViewType
	{
		None,
		SRV,
		UAV,
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

	struct LightAttribute
	{
		math::Vector4 Color;
		math::Vector4 Position;
		math::Vector4 Direction;

		enums::eLightType Type;
		float Radius;
		float Angle;
		int Padding;
	};

	struct tMtrlConst
	{
		int				arrInt[4];
		float			arrFloat[4];
		math::Vector2	arrV2[4];
		math::Vector4	arrV4[4];
	};

	struct tGlobal
	{
		float tDT;
		float tAccTime;
	};

	extern tGlobal GlobalData;

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

	struct tParticle
	{
		math::Vector4	vLocalPos;		// 오브젝트로부터 떨어진 거리
		math::Vector4	vWorldPos;		// 파티클 최종 월드위치
		math::Vector4	vWorldScale;	// 파티클 크기
		math::Vector4	vColor;			// 파티클 색상
		math::Vector4	vVelocity;		// 파티클 현재 속도
		math::Vector4	vForce;			// 파티클에 주어진 힘
		math::Vector4	vRandomForce;	// 파티클에 적용되는 랜덤방향 힘

		float   Age;			// 생존 시간
		float   PrevAge;		// 이전 프레임 생존 시간
		float   NomalizedAge;	// 수명대비 생존시간을 0~1로 정규화 한 값
		float	LifeTime;		// 수명
		float	Mass;			// 질량
		float   ScaleFactor;	// 추가 크기 배율

		int     Active;			// 파티클 활성화 여부
		int     pad;
	};


	struct tRWParticleBuffer
	{
		int		SpawnCount;			// 스폰 시킬 파티클 개수
		int		padding[3];
	};


	struct tParticleModule
	{
		// 스폰 모듈
		math::Vector4 vSpawnColor;
		math::Vector4	vSpawnScaleMin;
		math::Vector4	vSpawnScaleMax;
		math::Vector3	vBoxShapeScale;
		float	fSphereShapeRadius;
		int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
		int		SpawnRate;			// 초당 생성 개수
		int		Space;				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)
		float   MinLifeTime;		// 최소 수명
		float   MaxLifeTime;		// 최대 수명
		int     spawnpad[3];

		// Color Change 모듈
		math::Vector4	vStartColor;		// 초기 색상
		math::Vector4	vEndColor;			// 최종 색상

		// Scale Change 모듈
		float	StartScale;			// 초기 배율
		float	EndScale;			// 최종 배율	

		// 버퍼 최대크기
		int		iMaxParticleCount;
		int		ipad;

		// Add Velocity 모듈
		math::Vector4	vVelocityDir;
		int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
		float	OffsetAngle;
		float	Speed;
		//int     addvpad;

		// Drag 모듈 - 속도 제한
		float	StartDrag;
		float	EndDrag;

		// NoiseForce 모듈 - 랜덤 힘 적용	
		float	fNoiseTerm;		// 랜덤 힘 변경 간격
		float	fNoiseForce;	// 랜덤 힘 크기

		// Render 모듈
		int		VelocityAlignment;	// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
		int		VelocityScale;		// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
		float   vMaxSpeed;			// 최대 크기에 도달하는 속력
		math::Vector4	vMaxVelocityScale;	// 속력에 따른 크기 변화량 최대치
		int		renderpad;

		// Module Check
		int	ModuleCheck[(UINT)Lu::enums::PARTICLE_MODULE::END];
	};
}

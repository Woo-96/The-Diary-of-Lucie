// =================
// == ConstBuffer ==
// =================
cbuffer Transform : register(b0)
{
    row_major matrix WorldMatrix;
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
}

cbuffer Material : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
}

cbuffer Animator : register(b2)
{
    float2 SpriteLeftTop;
    float2 SpriteSize;
    float2 SpriteOffset;
    float2 AtlasSize;
    uint AnimationType;
    uint Reverse;
    uint2 Padding;
}

cbuffer GLOBAL : register(b3)
{
    float g_DT;
    float g_AccTime;
}

// =======================
// == Structured Buffer ==
// =======================
struct LightAttribute
{
    float4  Color;
    float4  Position;
    float4  Direction;
    
    uint    Type;
    float   Radius;
    float   Angle;
    int     Padding;
};
StructuredBuffer<LightAttribute> lightsAttribute : register(t13);

// Particle
struct tParticle
{
    float4 vLocalPos;
    float4 vWorldPos; // 파티클 위치
    float4 vWorldScale; // 파티클 크기
    float4 vColor; // 파티클 색상
    float4 vVelocity; // 파티클 현재 속도
    float4 vForce; // 파티클에 주어진 힘
    float4 vRandomForce; // 파티클에 적용되는 랜덤 힘

    float Age; // 생존 시간
    float PrevAge; // 이전 프레임 생존시간
    float NomalizedAge; // 수명대비 생존시간을 0~1로 정규화 한 값
    float LifeTime; // 수명
    float Mass; // 질량
    float ScaleFactor; // 추가 크기 배율
    
    int Active;
    int pad;
};


struct tParticleModule
{
	// 스폰 모듈
    float4 vSpawnColor;
    float4 vSpawnScaleMin;
    float4 vSpawnScaleMax;
    float3 vBoxShapeScale;
    float fSphereShapeRadius;
    int SpawnShapeType; // Sphere , Box
    int SpawnRate;
    int Space; // 0 World, 1 Local    
    float MinLifeTime;
    float MaxLifeTime;
    int3 spawnpad;

	// Color Change 모듈
    float4 vStartColor; // 초기 색상
    float4 vEndColor; // 최종 색상

	// Scale Change 모듈
    float StartScale; // 초기 크기
    float EndScale; // 최종 크기	

    int iMaxParticleCount;
    int ipad;
    
    // Add Velocity 모듈
    float4 vVelocityDir;
    int AddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float OffsetAngle;
    float Speed;
    int addvpad;
    
    // Drag 모듈
    float StartDrag;
    float EndDrag;
    
    // NoiseForce 모듈
    float fNoiseTerm;
    float fNoiseForce;
        
    // Render 모듈
    int VelocityAlignment; // 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
    int VelocityScale; // 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
    float vMaxSpeed; // 최대 크기에 도달하는 속력
    float4 vMaxVelocityScale; // 속력에 따른 크기 변화량 최대치
    int renderpad;

    
    // Module Check
    int Spawn;
    int ColorChange;
    int ScaleChange;
    int AddVelocity;
    
    int Drag;
    int NoiseForce;
    int Render;
    int modulepad;
};

// =============
// == Texture ==
// =============
Texture2D albedoTexture : register(t0);
Texture2D atlasTexture : register(t12);
Texture2D noiseTexture : register(t15);

// =============
// == Sampler ==
// =============
SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);


// ===========
// == Func ===
// ===========
void CalculateLight2D(in out float4 lightColor, float3 position, int idx)
{
    if (0 == lightsAttribute[idx].Type)
    {
        lightColor += lightsAttribute[idx].Color;
    }
    else if (1 == lightsAttribute[idx].Type)
    {
        float length = distance(position.xy, lightsAttribute[idx].Position.xy);
        
        if (length < lightsAttribute[idx].Radius)
        {
            float ratio = 1.0f - (length / lightsAttribute[idx].Radius);
            lightColor += lightsAttribute[idx].Color * ratio;
        }
    }
    else
    {
        
    }
}

// ============
// == Random ==
// ============
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += g_AccTime * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (g_AccTime /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += float4(_NoiseTex[pixel + offset + int2(j, i)] * GaussianFilter[i][j]).xyz;
        }
    }
    
    _vOut = vOut;
}
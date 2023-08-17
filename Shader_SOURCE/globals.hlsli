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
    float4 vWorldPos; // ��ƼŬ ��ġ
    float4 vWorldScale; // ��ƼŬ ũ��
    float4 vColor; // ��ƼŬ ����
    float4 vVelocity; // ��ƼŬ ���� �ӵ�
    float4 vForce; // ��ƼŬ�� �־��� ��
    float4 vRandomForce; // ��ƼŬ�� ����Ǵ� ���� ��

    float Age; // ���� �ð�
    float PrevAge; // ���� ������ �����ð�
    float NomalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
    float LifeTime; // ����
    float Mass; // ����
    float ScaleFactor; // �߰� ũ�� ����
    
    int Active;
    int pad;
};


struct tParticleModule
{
	// ���� ���
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

	// Color Change ���
    float4 vStartColor; // �ʱ� ����
    float4 vEndColor; // ���� ����

	// Scale Change ���
    float StartScale; // �ʱ� ũ��
    float EndScale; // ���� ũ��	

    int iMaxParticleCount;
    int ipad;
    
    // Add Velocity ���
    float4 vVelocityDir;
    int AddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float OffsetAngle;
    float Speed;
    int addvpad;
    
    // Drag ���
    float StartDrag;
    float EndDrag;
    
    // NoiseForce ���
    float fNoiseTerm;
    float fNoiseForce;
        
    // Render ���
    int VelocityAlignment; // 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
    int VelocityScale; // 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
    float vMaxSpeed; // �ִ� ũ�⿡ �����ϴ� �ӷ�
    float4 vMaxVelocityScale; // �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
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
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV �� ���
    vUV.y -= (sin((_NomalizedThreadID - (g_AccTime /*�׷��� ���� �̵� �ӵ�*/)) * 2.f * 3.1415926535f * 10.f /*�ݺ��ֱ�*/) / 2.f);
    
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
// =================
// == ConstBuffer ==
// =================
cbuffer Transform : register(b0)
{
    row_major matrix WorldMatrix;
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
}

cbuffer Animator : register(b2)
{
    float2  SpriteLeftTop;
    float2  SpriteSize;
    float2  SpriteOffset;
    float2  AtlasSize;
    uint    AnimationType;
    uint    Reverse;
    uint2   Padding;
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

//cbuffer ParticleSystem : register(b4)
//{
//    uint elementCount;
//    float elapsedTime;
//    int padd;
//    int padd2;
//}

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

struct Particle
{
    float4 Position;
    float4 Direction;
    
    float EndTime;
    float Time;
    float Speed;
    uint Active;
};
StructuredBuffer<Particle> particles : register(t14);


// =============
// == Texture ==
// =============
Texture2D albedoTexture : register(t0);
Texture2D atlasTexture : register(t12);

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

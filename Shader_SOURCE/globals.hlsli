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
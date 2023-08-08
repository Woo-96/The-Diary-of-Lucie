#include "globals.hlsli"

struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

#define bSampler        g_int_2
#define bINVINCIBLE     g_int_3
#define BLINK           g_float_3

float4 main(VSOut In) : SV_TARGET
{
    float4 color = float4(1.f, 0.f, 1.f, 1.f);
    
    if (AnimationType == 1)
    {
        if (Reverse == 1)
            In.UV.x = 1 - In.UV.x;
        
        float2 diff = (AtlasSize - SpriteSize) / 2.0f;
        float2 UV = (SpriteLeftTop - diff - SpriteOffset)
                + (AtlasSize * In.UV);
   
        if (UV.x < SpriteLeftTop.x || UV.x > SpriteLeftTop.x + SpriteSize.x
            || UV.y < SpriteLeftTop.y || UV.y > SpriteLeftTop.y + SpriteSize.y)
            discard;
        
        if (bINVINCIBLE)
        {
            color = atlasTexture.Sample(pointSampler, UV);
            color.a *= BLINK;
        }
        else
        {
            if (bSampler)
                color = atlasTexture.Sample(anisotropicSampler, UV);
            else
                color = atlasTexture.Sample(pointSampler, UV);
        }
    }
    else
    {
        color = albedoTexture.Sample(anisotropicSampler, In.UV);
    }
    
    //float4 lightColor = float4(1.f, 1.f, 1.f, 1.f);
    
    //for (int i = 0; i < 2; i++)
    //{
    //    CalculateLight2D(lightColor, In.WorldPos, i);
    //}
    
    //color *= lightColor;
    return color;
}
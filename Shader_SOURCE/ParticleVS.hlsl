#include "globals.hlsli"

struct VS_IN
{
    float3 vPos : POSITION;
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    uint iInstID : SV_InstanceID;
};

VS_OUT main(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
     
    output.vPos = _in.vPos;
    output.iInstID = _in.iInstID;
    
    return output;
}
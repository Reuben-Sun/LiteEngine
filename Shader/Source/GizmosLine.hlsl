#include "ShaderLibrary/LitInput.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
    float3 inColor : COLOR0;
};

struct Varyings
{
    float4 positionCS : SV_POSITION;
    float3 color : COLOR0;
};

cbuffer ubo : register(b0) { UBO ubo; }

Varyings MainVS(Attributes input)
{
    Varyings output = (Varyings) 0;
    output.positionCS = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(ubo.modelMatrix, float4(input.positionOS, 1.0f))));
    output.color = input.inColor;
	return output;
}

float4 MainPS(Varyings input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}
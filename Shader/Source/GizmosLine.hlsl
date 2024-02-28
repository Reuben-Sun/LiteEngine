#include "ShaderLibrary/LitInput.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
};

struct Varyings
{
    float4 positionCS : SV_POSITION;
};

struct PushConstant
{
    float4x4 modelMatrix;
    float3 color;
    float alpha;
};

cbuffer ubo : register(b0) { UBO ubo; }

[[vk::push_constant]]PushConstant pushConstant;

Varyings MainVS(Attributes input)
{
    Varyings output = (Varyings) 0;
    output.positionCS = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(pushConstant.modelMatrix, float4(input.positionOS, 1.0f))));
	return output;
}

float4 MainPS(Varyings input) : SV_TARGET
{
    return float4(pushConstant.color, 1.0f);
}
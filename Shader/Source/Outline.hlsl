#include "ShaderLibrary/LitInput.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
    float3 normalOS : NORMAL0;
};

struct Varyings
{
    float4 positionCS : SV_POSITION;
};

struct PushConstant
{
    float4x4 modelMatrix;
    float3 color;
    float outlineWidth;
};

cbuffer ubo : register(b0)
{
    UBO ubo;
}

[[vk::push_constant]]PushConstant pushConstant;

Varyings MainVS(Attributes input)
{
    Varyings output = (Varyings) 0;
    float3 outline_position = input.positionOS + input.normalOS * pushConstant.outlineWidth;
    output.positionCS = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(pushConstant.modelMatrix, float4(outline_position, 1.0f))));
    return output;
}

float4 MainPS(Varyings input) : SV_TARGET
{
    return float4(pushConstant.color, 1.0f);
}
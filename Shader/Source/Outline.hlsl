#include "ShaderLibrary/LitInput.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
    float3 normalOS : NORMAL0;
};

struct Varyings
{
    float4 positionCS : SV_POSITION;
    float3 color : TEXCOORD0;
};

struct PushConstant
{
    float4x4 modelMatrix;
    float4x4 invMVMatrix;
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
    float4 pos = mul(ubo.viewMatrix, mul(pushConstant.modelMatrix, float4(input.positionOS, 1.0f)));
    //float3 outline_position = input.positionOS + normalize(input.normalOS) * pushConstant.outlineWidth;
    float3 normal = mul((float3x3) pushConstant.invMVMatrix, input.normalOS);
    normal.z = -0.5;
    pos += float4(normal, 0) * pushConstant.outlineWidth;
    output.positionCS = mul(ubo.projectionMatrix, pos);
    output.color = normal;
    return output;
}

float4 MainPS(Varyings input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}
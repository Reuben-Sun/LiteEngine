#include "ShaderLibrary/LitInput.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
};

struct Varyings
{
    float4 positionCS : SV_POSITION;
    float3 uvw : TEXCOORD0;
};

struct PushConstant
{
    float4x4 modelMatrix;
};

cbuffer ubo : register(b0)
{
    UBO ubo;
}

[[vk::push_constant]]PushConstant pushConstant;

Varyings MainVS(Attributes input)
{
    Varyings output = (Varyings) 0;
    output.positionCS = mul(ubo.projectionMatrix, mul(pushConstant.modelMatrix,float4(input.positionOS, 1.0f)));
    output.uvw = input.positionOS;
    return output;
}

TextureCube _SkyboxMap : register(t1);
SamplerState _SkyboxMap_ST : register(s1);

float4 MainPS(Varyings input) : SV_TARGET
{
    float3 color = _SkyboxMap.Sample(_SkyboxMap_ST, input.uvw).xyz;
    return float4(color, 1.0f);
}
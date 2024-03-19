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
    output.positionCS = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(pushConstant.modelMatrix, float4(input.positionOS, 1.0f))));
    output.uvw = float3(input.positionOS.x, input.positionOS.z, -input.positionOS.y);
    return output;
}

Texture2D _BaseMap : register(t1);
SamplerState _BaseMap_ST : register(s1);

TextureCube _SkyboxMap : register(t2);

float4 MainPS(Varyings input) : SV_TARGET
{
    float3 color = _SkyboxMap.SampleLevel(_BaseMap_ST, input.uvw, 0).xyz;
    return float4(color, 1.0f);
}
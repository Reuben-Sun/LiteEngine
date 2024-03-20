#include "ShaderLibrary/Input.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
    float3 inColor : COLOR0;
    float2 texcoord : TEXCOORD0;
    float3 normalOS : NORMAL0;
    float4 tangentOS : TANGENT0;
};

struct Varyings
{
    float4 positionCS : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normalWS : NORMAL0;
    float3 positionWS : TEXCOORD1;
    float4 tangentOS : TEXCOORD2;
};

struct PushConstant
{
    float4x4 modelMatrix;
    float3 baseColor;
    float metallic;
    float3 emissionColor;
    float roughness;
    uint textureEnable;
    uint debugMode;
    float2 padding;
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
    output.uv = input.texcoord;
    output.normalWS = normalize(mul(pushConstant.modelMatrix, float4(input.normalOS, 0.0f)).xyz);
    output.positionWS = mul(pushConstant.modelMatrix, float4(input.positionOS, 1.0f)).xyz;
    output.tangentOS = float4(input.tangentOS.xyz, 1.0f);
    return output;
}

Texture2D _BaseMap : register(t1);
SamplerState _BaseMap_ST : register(s1);

float4 MainPS(Varyings input) : SV_TARGET
{
    return float4(1.0, 0.0, 0.0, 0.5);
}
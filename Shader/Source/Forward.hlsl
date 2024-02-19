struct Attributes
{
    [[vk::location(0)]] float3 positionOS : POSITION0;
    [[vk::location(1)]] float3 inColor : COLOR0;
    [[vk::location(2)]] float2 texcoord : TEXCOORD0;
};

struct Varyings
{
	float4 positionCS : SV_POSITION;
    [[vk::location(0)]] float2 uv : TEXCOORD0;
};

struct UBO
{
	float4x4 modelMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

cbuffer ubo : register(b0) { UBO ubo; }

Varyings MainVS(Attributes input)
{
    Varyings output = (Varyings) 0;
    output.positionCS = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(ubo.modelMatrix, float4(input.positionOS, 1.0f))));
    output.uv = input.texcoord;
	return output;
}

Texture2D _BaseMap : register(t1);
SamplerState _BaseMap_ST : register(s1);

float4 MainPS(Varyings input) : SV_TARGET
{
    return _BaseMap.Sample(_BaseMap_ST, input.uv);
}
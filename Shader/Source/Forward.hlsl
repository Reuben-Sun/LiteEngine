struct VSInput
{
    [[vk::location(0)]] float3 Pos : POSITION0;
    [[vk::location(1)]] float3 inColor : COLOR0;
    [[vk::location(2)]] float2 UV : TEXCOORD0;
};

struct VSOutput
{
	float4 Pos : SV_POSITION;
    [[vk::location(0)]] float2 UV : TEXCOORD0;
};

struct UBO
{
	float4x4 modelMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

cbuffer ubo : register(b0) { UBO ubo; }

VSOutput MainVS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.Pos = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(ubo.modelMatrix, float4(input.Pos, 1.0f))));
    output.UV = input.UV;
	return output;
}

Texture2D textureColor : register(t1);
SamplerState samplerColor : register(s1);

float4 MainPS(VSOutput input) : SV_TARGET
{
    return textureColor.Sample(samplerColor, input.UV);
}
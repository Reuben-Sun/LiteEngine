struct Attributes
{
    [[vk::location(0)]] float3 positionOS : POSITION0;
    [[vk::location(1)]] float3 inColor : COLOR0;
};

struct Varyings
{
    float4 positionCS : SV_POSITION;
    [[vk::location(0)]] float3 color : COLOR0;
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
    output.color = input.inColor;
	return output;
}

float4 MainPS(Varyings input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}
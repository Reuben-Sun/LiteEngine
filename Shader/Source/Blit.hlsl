struct VSOutput
{
	float4 Pos : SV_POSITION;
    [[vk::location(0)]] float2 UV : TEXCOORD0;
};

VSOutput MainVS(uint VertexIndex : SV_VertexID)
{
	VSOutput output = (VSOutput)0;
	output.UV = float2((VertexIndex << 1) & 2, VertexIndex & 2);
	output.Pos = float4(output.UV * 2.0f - 1.0f, 0.0f, 1.0f);
	return output;
}

Texture2D textureColor : register(t1);
SamplerState samplerColor : register(s1);

float4 MainPS(VSOutput input) : SV_TARGET
{
    return textureColor.Sample(samplerColor, input.UV);
}
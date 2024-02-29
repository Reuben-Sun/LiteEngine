struct Varyings
{
    float4 positionCS : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Varyings MainVS(uint VertexIndex : SV_VertexID)
{
	Varyings output = (Varyings)0;
    output.uv = float2((VertexIndex << 1) & 2, VertexIndex & 2);
    output.positionCS = float4(output.uv * 2.0f - 1.0f, 0.0f, 1.0f);
	return output;
}

Texture2D _BaseMap : register(t0);
SamplerState _BaseMap_ST : register(s0);

float4 MainPS(Varyings input) : SV_TARGET
{
    return _BaseMap.Sample(_BaseMap_ST, input.uv);
}
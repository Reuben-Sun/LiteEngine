#include "ShaderLibrary/LitInput.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
    float3 inColor : COLOR0;
    float2 texcoord : TEXCOORD0;
    float3 normalOS : NORMAL0;
};

struct Varyings
{
	float4 positionCS : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normalWS : NORMAL0;
    float3 positionWS : TEXCOORD1;
};

cbuffer ubo : register(b0) { UBO ubo; }

Varyings MainVS(Attributes input)
{
    Varyings output = (Varyings) 0;
    output.positionCS = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(ubo.modelMatrix, float4(input.positionOS, 1.0f))));
    output.uv = input.texcoord;
    output.normalWS = normalize(mul(ubo.modelMatrix, float4(input.normalOS, 0.0f)).xyz);
    output.positionWS = mul(ubo.modelMatrix, float4(input.positionOS, 1.0f)).xyz;
	return output;
}

Texture2D _BaseMap : register(t1);
SamplerState _BaseMap_ST : register(s1);

float4 MainPS(Varyings input) : SV_TARGET
{
    float3 lightDir = ubo.dirLight.direction.xyz;
    float3 lightColor = ubo.dirLight.color.xyz * ubo.dirLight.intensity;
    float3 ambientColor = float3(0.1f, 0.1f, 0.1f);
    float3 albedo = _BaseMap.Sample(_BaseMap_ST, input.uv).xyz;
    float3 diffuse = max(dot(input.normalWS, lightDir), 0.0f) * lightColor * albedo;
    float3 reflectDir = reflect(-lightDir, input.normalWS);
    float3 viewDir = normalize(ubo.cameraPosition.xyz - input.positionWS);
    float3 halfDir = normalize(lightDir + viewDir);
    float3 specular = pow(max(dot(reflectDir, viewDir), 0.0f), 32.0f) * lightColor;
    //return float4(lightDir, 1.0);
    return float4(ambientColor + diffuse + specular, 1.0);
}

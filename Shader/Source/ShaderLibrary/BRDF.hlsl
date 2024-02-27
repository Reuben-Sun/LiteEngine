struct BRDFData
{
    float3 albedo;
    float metallic;
    float3 emissionColor;
    float roughness;
};

struct Input
{
    float3 L;   // light dir
    float3 H;   // half dir
    float VoH;
    float NoV;
    float NoL;
    float NoH;
    float3 radiance;
};

static const float PI = 3.14159265359f;

float4 BRDF(BRDFData data, Input input)
{
    float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), data.albedo, data.metallic);
    // F Schlick Fresnel
    float3 F_Schlick = F0 + (1 - F0) * pow(1 - input.VoH, 5.0);
    // lambert diffuse
    float3 Kd = (1 - F_Schlick) * (1 - data.metallic);
    float3 brdfDiffuse = data.albedo * Kd;
    float3 diffuseColor = brdfDiffuse * input.radiance;
    // D GGX
    float a = data.roughness * data.roughness;
    float a2 = a * a;
    float d = (input.NoH * a2 - input.NoH) * input.NoH + 1;
    float D_GGX = a2 / (PI * d * d);
    // G
    float k = (data.roughness + 1) * (data.roughness + 1) / 8;
    float GV = input.NoV / (input.NoV * (1 - k) + k);
    float GL = input.NoL / (input.NoL * (1 - k) + k);
    float G_GGX = GV * GL;
    // specular
    float3 brdfSpecular = F_Schlick * D_GGX * G_GGX / (4 * input.NoV * input.NoL);
    float3 specularColor = brdfSpecular * input.radiance * PI;
    // result
    float3 result = diffuseColor + specularColor;
    return float4(result, 1.0f);
}
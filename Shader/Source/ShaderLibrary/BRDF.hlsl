#ifndef BRDF_HLSL_INCLUDED
#define BRDF_HLSL_INCLUDED
struct BRDFData
{
    float3 albedo;
    float metallic;
    float reflectivity;
    float3 diffuse;
    float3 specular;
    float roughness;
    float roughness2;
    float grazingTerm;
};

static const float PI = 3.14159265359f;

inline float OneMinusReflectivityMetallic(float metallic)
{
    float oneMinusDielectricSpec = 1.0 - 0.04;
    return oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
}

float3 SpecularBRDF(BRDFData data, float3 normalWS, float3 viewDir, float3 lightDir)
{
    float3 H = normalize(viewDir + lightDir);
    float NoV = max(0.0f, dot(normalWS, viewDir));
    float NoL = max(0.0f, dot(normalWS, lightDir));
    float NoH = max(0.0f, dot(normalWS, H));
    float VoH = max(0.0f, dot(viewDir, H));
    
    float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), data.albedo, data.metallic);
    // F Schlick Fresnel
    float3 F_Schlick = F0 + (1 - F0) * pow(1 - VoH, 5.0);

    // D GGX
    float a = data.roughness * data.roughness;
    float a2 = a * a;
    float d = (NoH * a2 - NoH) * NoH + 1;
    float D_GGX = a2 / (PI * d * d);
    // G
    float k = (data.roughness + 1) * (data.roughness + 1) / 8;
    float GV = NoV / (NoV * (1 - k) + k);
    float GL = NoL / (NoL * (1 - k) + k);
    float G_GGX = GV * GL;

    float3 brdfSpecular = F_Schlick * D_GGX * G_GGX / (4 * NoV * NoL);
    return brdfSpecular;
}
#endif
#include "Marco.h"
#include <glm/glm.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

float DistributionGGX(glm::vec3 N, glm::vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = glm::clamp(dot(N, H), 0.0f, 1.0f);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;

	return nom / denom;
}

float RadicalInverse_VdC(uint32_t bits)
{
	bits = bits << 16u | bits >> 16u;
	bits = (bits & 0x55555555u) << 1u | (bits & 0xAAAAAAAAu) >> 1u;
	bits = (bits & 0x33333333u) << 2u | (bits & 0xCCCCCCCCu) >> 2u;
	bits = (bits & 0x0F0F0F0Fu) << 4u | (bits & 0xF0F0F0F0u) >> 4u;
	bits = (bits & 0x00FF00FFu) << 8u | (bits & 0xFF00FF00u) >> 8u;
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

glm::vec2 Hammersley(uint32_t i, uint32_t N)
{
	return glm::vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

glm::vec3 ImportanceSampleGGX(glm::vec2 Xi, float roughness, glm::vec3 N)
{
	float a = roughness * roughness;
	float phi = 2.0 * PI * Xi.x;
	float cos_theta = glm::sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
	float sin_theta = glm::sqrt(1.0 - cos_theta * cos_theta);
	glm::vec3 H;
	H.x = cos(phi) * sin_theta;
	H.y = sin(phi) * sin_theta;
	H.z = cos_theta;
	glm::vec3 up = glm::abs(N.z) < 0.999 ? glm::vec3(0.0, 0.0, 1.0) : glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 tangent = glm::normalize(glm::cross(up, N));
	glm::vec3 bitangent = glm::cross(N, tangent);
	glm::vec3 sample_vec = tangent * H.x + bitangent * H.y + N * H.z;
	return glm::normalize(sample_vec);
}

glm::vec3 calculate(int mip_size, glm::vec3 N, glm::vec3 V, std::vector<stbi_uc*>& cubemap)
{
	glm::vec3 result;
	int sample_count = 1024;
	float roughness = glm::clamp((float)mip_size / 10.0, 0.0, 1.0);
	float total_weight = 0.0f;
	for (int i = 0; i < sample_count; i++)
	{
		glm::vec2 Xi = Hammersley(i, sample_count);
		glm::vec3 H = ImportanceSampleGGX(Xi, roughness, N);
		glm::vec3 L = glm::normalize(2.0f * glm::dot(V, H) * H - V);
		float NdotL = glm::max(glm::dot(N, L), 0.0f);
		if (NdotL > 0.0)
		{
			float D = DistributionGGX(N, H, roughness);
			float NdotH = glm::max(glm::dot(N, H), 0.0f);
			float HdotV = glm::max(glm::dot(H, V), 0.0f);
			float pdf = D * NdotH / (4.0f * HdotV) + 0.00001;
			float resolution = 1024.0f;
			float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
			float saSample = 1.0 / (float(sample_count) * pdf + 0.0001);
			float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
			
			float sx = L.x;
			float sy = L.y;
			float sz = L.z;
			int point = int((sy+1)/2 * 1024) * 4 + ((sx+1)/2 * 4);
			float cx = cubemap[sz][point];
			float cy = cubemap[sz][point + 1];
			float cz = cubemap[sz][point + 2];
			result += glm::vec3(cx, cy, cz) * NdotL;
			total_weight += NdotL;
		}
	}
	result /= total_weight;
	return result;
}

#include <vector>
int main(int argc, char* argv[])
{
    std::string root_path = "E:\\ev\\dev\\wolfgang\\_games\\proven_ground\\chaos\\_source\\_engine\\source\\unit_test\\assets\\skybox\\";
    std::vector<std::string> skybox_images = {
		root_path + "nx.png",
		root_path + "px.png",
		root_path + "ny.png",
		root_path + "py.png",
		root_path + "nz.png",
		root_path + "pz.png"
	};
	std::vector<std::string> out_skybox_images = {
		root_path + "nx_o.png",
		root_path + "px_o.png",
		root_path + "ny_o.png",
		root_path + "py_o.png",
		root_path + "nz_o.png",
		root_path + "pz_o.png"
	};
    // Load an image
    int width, height, channels;
    std::vector<stbi_uc*> pixels;
	for (auto& path : skybox_images)
	{
		stbi_uc* pixel = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (!pixel)
		{
			LOG_ERROR("failed to load texture image!");
		}
		pixels.push_back(pixel);
	}
	std::vector<unsigned char*> datas = { 
		new unsigned char[width * height * 4], 
		new unsigned char[width * height * 4],
		new unsigned char[width * height * 4],
		new unsigned char[width * height * 4],
		new unsigned char[width * height * 4],
		new unsigned char[width * height * 4],
	};

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float texture_size = 1.0f / width;
			glm::vec2 uv = glm::vec2(x + 0.5f, y + 0.5f) * texture_size - 0.5f;
			std::vector<glm::vec3> sampler_normal = {
				glm::vec3(0.5f, -uv[1], -uv[0]),
				glm::vec3(-0.5f, -uv[1], uv[0]),
				glm::vec3(uv[0], 0.5f, uv[1]),
				glm::vec3(uv[0], -0.5f, -uv[1]),
				glm::vec3(uv[0], -uv[1], 0.5f),
				glm::vec3(-uv[0], -uv[1], -0.5f)
			};
			for (int z = 0; z < pixels.size(); z++)
			{
				LOG_INFO("x: {}, y: {}, z: {}", x, y, z);	
				glm::vec3 N = glm::normalize(sampler_normal[z]);
				glm::vec3 R = N;
				glm::vec3 V = N;
				datas[z][y * 1024 + x] = calculate(1, N, V, pixels)[0];
				datas[z][y * 1024 + x + 1] = calculate(1, N, V, pixels)[1];
				datas[z][y * 1024 + x + 2] = calculate(1, N, V, pixels)[2];
				datas[z][y * 1024 + x + 3] = 255;
			}
			
		}
	}


	for (int i = 0; i < pixels.size(); i++)
	{
		stbi_image_free(pixels[i]);
	}
	for (int i = 0; i < datas.size(); i++)
	{
		stbi_write_png(out_skybox_images[i].c_str(), width, height, 4, datas[i], width * 4);
		delete[] datas[i];
	}
    return 0;
}
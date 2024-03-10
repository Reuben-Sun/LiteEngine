#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIUniformBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHITextureImage.h"
#include "Geometry/Light.h"
#include "Render/Resources/RenderScene.h"
#include "Render/Resources/UBO.h"
#include "Render/Resources/PushConstant.h"

namespace ToolEngine
{
	class CullingResult
	{
	public:
		CullingResult(RHIDevice& device, RHIDescriptorSetLayout& layout, RHIDescriptorPool& pool);
		~CullingResult();

		// TODO: cull with camera
		void cull(RenderScene& scene);

		RHIVertexBuffer& getVertexBuffer(const std::string& sub_model_name);
		RHIIndexBuffer& getIndexBuffer(const std::string& sub_model_name);
		RHIDescriptorSet& getDescriptorSet(const std::string& material_name);
		PushConstant getPushConstant(const std::string& material_name);

		std::unordered_map<std::string, std::vector<std::string>> m_model_name_to_sub_model_name;
	private:
		RHIDevice& m_device;
		RHIDescriptorSetLayout& m_ubo_descriptor_set_layout;
		RHIDescriptorPool& m_ubo_descriptor_pool;


		// TODO: unload useless buffer
		std::unordered_map<std::string, std::unique_ptr<RHIVertexBuffer>> m_sub_mesh_name_to_vertex_buffer;
		std::unordered_map<std::string, std::unique_ptr<RHIIndexBuffer>> m_sub_mesh_name_to_index_buffer;

		std::unordered_map<std::string, PushConstant> m_material_name_to_push_constant;
		std::unordered_map<std::string, std::unique_ptr<RHIDescriptorSet>> m_material_name_to_descriptor_set;
		std::unordered_map<std::string, std::unique_ptr<RHITextureImage>> m_texture_name_to_image;

		const uint32_t TEXTURE_MIN_BINDING = 1;
		const uint32_t TEXTURE_MAX_BINDING = 5;
	};
}
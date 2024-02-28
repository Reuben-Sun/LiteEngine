#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "Geometry/RenderScene.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIUniformBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHITextureImage.h"
#include "Geometry/Light.h"
#include "Geometry/UBO.h"

namespace ToolEngine
{
	class CullingResult
	{
	public:
		CullingResult(RHIDevice& device, RHIDescriptorSetLayout& layout, RHIDescriptorPool& pool);
		~CullingResult();

		// TODO: cull with camera
		void cull(RenderScene& scene);

		RHIVertexBuffer& getVertexBuffer(const std::string& model_name);
		RHIIndexBuffer& getIndexBuffer(const std::string& model_name);
		RHIUniformBuffer& getGlobalUBO() { return *m_global_ubo; }
		RHIDescriptorSet& getDescriptorSet(const std::string& model_name);
		Light& getDirLight() { return m_dir_light; }

		
	private:
		RHIDevice& m_device;
		RHIDescriptorSetLayout& m_ubo_descriptor_set_layout;
		RHIDescriptorPool& m_ubo_descriptor_pool;
		// TODO: use guid to replace std::string
		// TODO: unload useless buffer
		std::unordered_map<std::string, std::unique_ptr<RHIVertexBuffer>> m_model_name_to_vertex_buffer;
		std::unordered_map<std::string, std::unique_ptr<RHIIndexBuffer>> m_model_name_to_index_buffer;
		std::unordered_map<std::string, std::unique_ptr<RHIDescriptorSet>> m_material_name_to_descriptor_set;
		std::unordered_map<std::string, std::unique_ptr<RHITextureImage>> m_texture_name_to_image;
		std::unique_ptr<RHIUniformBuffer> m_global_ubo;
		Light m_dir_light;
		std::unordered_map<std::string, std::string> m_model_name_to_material_name;
	};
}
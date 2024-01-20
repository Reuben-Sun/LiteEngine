#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "Geometry/RenderScene.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"

namespace ToolEngine
{
	class CullingResult
	{
	public:
		CullingResult(RHIDevice& device);
		~CullingResult();

		// TODO: cull with camera
		void cull(RenderScene& scene);

		RHIVertexBuffer& getVertexBuffer(const std::string& model_name);
		RHIIndexBuffer& getIndexBuffer(const std::string& model_name);

	private:
		RHIDevice& m_device;
		// TODO: use guid to replace std::string
		// TODO: unload useless buffer
		std::unordered_map<std::string, std::unique_ptr<RHIVertexBuffer>> m_model_name_to_vertex_buffer;
		std::unordered_map<std::string, std::unique_ptr<RHIIndexBuffer>> m_model_name_to_index_buffer;
	};
}
#include "CullingResult.h"

namespace ToolEngine
{
	CullingResult::CullingResult(RHIDevice& device): m_device(device)
	{
	}
	CullingResult::~CullingResult()
	{
		m_model_name_to_index_buffer.clear();
		m_model_name_to_vertex_buffer.clear();
	}
	void CullingResult::cull(RenderScene& scene)
	{
		// TODO: currrent without culling
		// create buffer
		for (int i = 0; i < scene.mesh_name_list.size(); i++)
		{
			if(m_model_name_to_index_buffer.find(scene.mesh_name_list[i]) == m_model_name_to_index_buffer.end())
			{
				m_model_name_to_index_buffer.emplace(scene.mesh_name_list[i], std::make_unique<RHIIndexBuffer>(m_device, scene.mesh_list[i].index_buffer));
				m_model_name_to_vertex_buffer.emplace(scene.mesh_name_list[i], std::make_unique<RHIVertexBuffer>(m_device, scene.mesh_list[i].vertex_buffer));
			}
		}
	}
	RHIVertexBuffer& CullingResult::getVertexBuffer(const std::string& model_name)
	{
		auto it = m_model_name_to_vertex_buffer.find(model_name);
		RHIVertexBuffer& vertex_buffer_ref = *(it->second.get());
		return vertex_buffer_ref;
	}
	RHIIndexBuffer& CullingResult::getIndexBuffer(const std::string& model_name)
	{
		auto it = m_model_name_to_index_buffer.find(model_name);
		RHIIndexBuffer& index_buffer_ref = *(it->second.get());
		return index_buffer_ref;
	}
}
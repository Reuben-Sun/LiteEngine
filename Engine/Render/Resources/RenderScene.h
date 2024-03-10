#pragma once

#include "Marco.h"
#include "UBO.h"
#include "PushConstant.h"
#include "Geometry/Mesh.h"
#include "Geometry/Material.h"
#include "Geometry/Camera.h"
#include "Geometry/Bounding.h"
#include "Geometry/Light.h"
#include "Core/Math/Transform.h"
#include "Logic/ECS/LogicScene.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIUniformBuffer.h"
#include "RHI/Public/RHITextureImage.h"

namespace ToolEngine
{
	struct RenderEntity
	{
		uint32_t go_id;
		std::string mesh_name;
		std::string mesh_path;
		std::vector<std::string> material_names;
		Transform transform;
		Bounding bounding;
	};

	class SceneResources
	{
	public:
		SceneResources(RHIDevice& device);
		~SceneResources();

		void tick(std::vector<RenderEntity>& entities);

		RHIVertexBuffer& getVertexBuffer(const std::string& sub_model_name);
		RHIIndexBuffer& getIndexBuffer(const std::string& sub_model_name);

		std::unique_ptr<RHIUniformBuffer> m_global_ubo;
		std::unique_ptr<RHITextureImage> m_global_default_texture;
		std::unique_ptr<RHITextureCube> m_skybox_texture;
		Light m_dir_light;
		std::unordered_map<std::string, std::vector<std::string>> m_model_name_to_sub_model_name;
	private:
		RHIDevice& m_device;

		std::unordered_map<std::string, std::unique_ptr<RHIVertexBuffer>> m_sub_mesh_name_to_vertex_buffer;
		std::unordered_map<std::string, std::unique_ptr<RHIIndexBuffer>> m_sub_mesh_name_to_index_buffer;
	};

	struct RenderScene
	{
		std::vector<RenderEntity> render_entities;
		Camera camera;
		std::unique_ptr<SceneResources> m_resources{ nullptr };
		void init(LogicScene& logic_scene);
		void tick();
	};
}

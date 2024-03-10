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

		std::unique_ptr<RHIUniformBuffer> m_global_ubo;
		std::unique_ptr<RHITextureImage> m_global_default_texture;
		std::unique_ptr<RHITextureCube> m_skybox_texture;
		Light m_dir_light;
	private:
		RHIDevice& m_device;
	};

	struct RenderScene
	{
		std::vector<RenderEntity> render_entities;
		Camera camera;
		std::unique_ptr<SceneResources> m_resources{ nullptr };
		void init(LogicScene& logic_scene);
	};
}

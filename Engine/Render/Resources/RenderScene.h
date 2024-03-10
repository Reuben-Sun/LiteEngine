#pragma once

#include "Marco.h"
#include "Geometry/Mesh.h"
#include "Geometry/Material.h"
#include "Geometry/Camera.h"
#include "Geometry/Bounding.h"
#include "Core/Math/Transform.h"

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
	struct RenderScene
	{
		std::vector<RenderEntity> render_entities;
		Camera camera;
	};
}

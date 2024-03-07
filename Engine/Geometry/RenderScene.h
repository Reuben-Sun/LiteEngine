#pragma once

#include "Marco.h"
#include "Mesh.h"
#include "Material.h"
#include "Core/Math/Transform.h"
#include "Geometry/Camera.h"
#include "Bounding.h"

namespace ToolEngine
{
	struct RenderEntity
	{
		uint32_t go_id;
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

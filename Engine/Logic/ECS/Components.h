#pragma once

#include "Marco.h"
#include "Core/Math/Transform.h"
#include "Geometry/Bounding.h"
#include <entt/entt.hpp>

namespace ToolEngine
{
	struct InfoComponent
	{
		std::string name;
		uint32_t id;
	};

	struct MeshComponent
	{
		std::string mesh_path;
	};

	struct MaterialComponent
	{
		std::string material_path;
	};

	struct TransformComponent
	{
		Transform transform;
	};

	struct BoundingComponent
	{
		Bounding bounding;
	};
}
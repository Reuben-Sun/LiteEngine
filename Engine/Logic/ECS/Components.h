#pragma once

#include "Marco.h"
#include "Core/Math/Transform.h"

namespace ToolEngine
{
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
}
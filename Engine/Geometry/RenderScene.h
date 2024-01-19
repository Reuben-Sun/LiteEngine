#pragma once

#include "Marco.h"
#include "Mesh.h"
#include "Core/Math/Transform.h"

namespace ToolEngine
{
	struct RenderScene
	{
		std::vector<Mesh> mesh_list;
		std::vector<Transform> mesh_transform_list;
	};
}

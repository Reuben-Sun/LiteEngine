#pragma once

#include "Marco.h"
#include "Core/Math/Transform.h"
#include "Geometry/Bounding.h"
#include <entt/entt.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include "Geometry/Camera.h"

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
		std::vector<std::string> material_paths;
	};

	struct TransformComponent
	{
		Transform transform;
	};

	struct BoundingComponent
	{
		Bounding bounding;
	};

	struct PhysicsComponent
	{
		JPH::BodyID body_id;
	};

	struct CameraComponent
	{
		Camera camera;
	};

	struct UIInfoComponent
	{
		glm::vec4 scene_bounding;
	};
}
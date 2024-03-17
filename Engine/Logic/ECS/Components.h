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
		uint32_t current_mouse_x;
		uint32_t current_mouse_y;
		bool pickup_update = false;
		std::string pickup_entity_mesh_name;
		glm::vec3 test_pos;

		bool inSceneBounding() const
		{
			bool result = scene_bounding.x < current_mouse_x && current_mouse_x < scene_bounding.z &&
				scene_bounding.y < current_mouse_y && current_mouse_y < scene_bounding.w;
			return result;
		}

		glm::vec2 getScreenUV() const
		{
			float u = (current_mouse_x - scene_bounding.x) / (scene_bounding.z - scene_bounding.x);
			float v = (current_mouse_y - scene_bounding.y) / (scene_bounding.w - scene_bounding.y);
			return glm::vec2(u, v);
		}
	};
}
#pragma once

#include "Marco.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include "Geometry/RenderScene.h"

namespace ToolEngine
{
	class PhysicsManager
	{
	public:
		PhysicsManager(RenderScene& scene);
		~PhysicsManager();

		void tick();
	private:
		RenderScene& m_scene;

		JPH::PhysicsSystem* m_physics_system{ nullptr };
		JPH::JobSystem* m_job_system{ nullptr };
		JPH::TempAllocator* m_temp_allocator{ nullptr };
		JPH::BroadPhaseLayerInterface* m_jolt_broad_phase_layer_interface{ nullptr };

		// scene setting
		uint32_t m_max_body_count{ 10240 };
		uint32_t m_body_mutex_count{ 0 };
		uint32_t m_max_body_pairs{ 65536 };
		uint32_t m_max_contact_constraints{ 10240 };

		// job setting
		uint32_t m_max_job_count{ 1024 };
		uint32_t m_max_barrier_count{ 8 };
		uint32_t m_max_concurrent_job_count{ 4 };

		glm::vec3 m_gravity{ 0.f, 0.f, -9.8f };

	};
}
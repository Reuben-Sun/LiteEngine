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
#include "Logic/ECS/LogicScene.h"
#include "JoltUtils.h"
#include "Listener.h"

namespace ToolEngine
{
	class PhysicsManager
	{
	public:
		PhysicsManager(LogicScene& scene);
		~PhysicsManager();

		void tick();
		
	private:
		LogicScene& m_scene;

		BPLayerInterfaceImpl layer_interface;
		ObjectVsBroadPhaseLayerFilterImpl ob_layer_filter;
		ObjectLayerPairFilterImpl oo_layer_filter;

		MyBodyActivationListener* body_activation_listener;
		MyContactListener* contact_listener;

		JPH::PhysicsSystem* m_physics_system{nullptr};
		JPH::JobSystem* m_job_system{ nullptr };
		JPH::TempAllocator* m_temp_allocator{ nullptr };

		// scene setting
		const uint32_t m_max_body_count{ 1024 };
		const uint32_t m_body_mutex_count{ 0 };
		const uint32_t m_max_body_pairs{ 1024 };
		const uint32_t m_max_contact_constraints{ 1024 };

		// job setting
		const uint32_t m_max_job_count{ 1024 };
		const uint32_t m_max_barrier_count{ 8 };
		const uint32_t m_max_concurrent_job_count{ 4 };

		JPH::Vec3 m_gravity{ 0.f, 0.f, -9.8f };

		static void TraceImpl(const char* inFMT, ...);
		static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, unsigned int inLine);

		void createPhysicsScene();

		float m_fix_update_rate = 120.0f;
		float getFixedDeltaTime() const { return 1.0f / m_fix_update_rate; }
		float m_accumulator = 0.0f;
		void fixedTick();
	};
}
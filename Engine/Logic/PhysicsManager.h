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
	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};
	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr unsigned int NUM_LAYERS(2);
	};

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl();
		virtual unsigned int GetNumBroadPhaseLayers() const override;
		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
	private:
		JPH::BroadPhaseLayer m_object_to_broad_phase[Layers::NUM_LAYERS];
	};

	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
	};

	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};

	class PhysicsManager
	{
	public:
		PhysicsManager(RenderScene& scene);
		~PhysicsManager();

		void tick();
	private:
		RenderScene& m_scene;

		BPLayerInterfaceImpl layer_interface;
		ObjectVsBroadPhaseLayerFilterImpl ob_layer_filter;
		ObjectLayerPairFilterImpl oo_layer_filter;

		JPH::PhysicsSystem* m_physics_system{nullptr};
		JPH::JobSystem* m_job_system{ nullptr };
		JPH::TempAllocator* m_temp_allocator{ nullptr };
		JPH::BroadPhaseLayerInterface* m_jolt_broad_phase_layer_interface{ nullptr };

		// scene setting
		uint32_t m_max_body_count{ 1024 };
		uint32_t m_body_mutex_count{ 0 };
		uint32_t m_max_body_pairs{ 1024 };
		uint32_t m_max_contact_constraints{ 1024 };

		// job setting
		uint32_t m_max_job_count{ 1024 };
		uint32_t m_max_barrier_count{ 8 };
		uint32_t m_max_concurrent_job_count{ 4 };

		glm::vec3 m_gravity{ 0.f, 0.f, -9.8f };

		static void TraceImpl(const char* inFMT, ...);
		static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, unsigned int inLine);

	};
}
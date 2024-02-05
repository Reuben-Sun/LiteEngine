#include "PhysicsManager.h"
#include <cstdarg>
#include "Core/Time/Time.h"

namespace ToolEngine
{
	PhysicsManager::PhysicsManager(RenderScene& scene): m_scene(scene)
	{
		// Init
		JPH::RegisterDefaultAllocator();
		JPH::Trace = PhysicsManager::TraceImpl;
		JPH::AssertFailed = PhysicsManager::AssertFailedImpl;
		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();
		m_temp_allocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
		m_job_system = new JPH::JobSystemThreadPool(m_max_job_count, m_max_barrier_count, m_max_concurrent_job_count);
		m_physics_system = new JPH::PhysicsSystem();
		m_physics_system->Init(m_max_body_count, m_max_body_count, m_max_body_pairs, m_max_contact_constraints,
			layer_interface, ob_layer_filter, oo_layer_filter);
		body_activation_listener = new MyBodyActivationListener();
		m_physics_system->SetBodyActivationListener(body_activation_listener);
		contact_listener = new MyContactListener();
		m_physics_system->SetContactListener(contact_listener);
		
		// add something
		JPH::BodyInterface& body_interface = m_physics_system->GetBodyInterface();
		JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));
		JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
		JPH::ShapeRefC floor_shape = floor_shape_result.Get();
		JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(0.0f, -1.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);
		JPH::Body* floor = body_interface.CreateBody(floor_settings);
		body_interface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);


		m_physics_system->OptimizeBroadPhase();

	}
	PhysicsManager::~PhysicsManager()
	{
	}
	void PhysicsManager::tick()
	{
		float dt = Time::getInstance().getDeltaTime();
		m_physics_system->Update(dt, 1, m_temp_allocator, m_job_system);
	}
	void PhysicsManager::TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);

		LOG_INFO("{0}", buffer);
	}
	bool PhysicsManager::AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, unsigned int inLine)
	{
		LOG_ERROR("{0}:{1}: ({2}) {3}", inFile, inLine, inExpression, inMessage);
		return false;
	}
	
}
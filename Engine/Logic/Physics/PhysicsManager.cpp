#include "PhysicsManager.h"
#include <cstdarg>

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
	}
	PhysicsManager::~PhysicsManager()
	{
	}
	void PhysicsManager::tick()
	{
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
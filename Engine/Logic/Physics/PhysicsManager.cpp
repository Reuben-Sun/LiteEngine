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
		m_physics_system->SetGravity(m_gravity);
		
		// create physical scene
		JPH::BodyInterface& body_interface = m_physics_system->GetBodyInterface();
		/*for (uint32_t i = 0; i < scene.go_id_list.size(); i++)
		{
			uint32_t go_id = scene.go_id_list[i];
			Bounding& bounding = scene.bounding_list[i];
			if (bounding.type == BoundingType::Box)
			{
				JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(bounding.data[0], bounding.data[1], bounding.data[2]));
				JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
				JPH::ShapeRefC floor_shape = floor_shape_result.Get();
				JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(bounding.position[0], bounding.position[1], bounding.position[2]), 
					JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);
				JPH::Body* box = body_interface.CreateBody(floor_settings);
				body_interface.AddBody(box->GetID(), JPH::EActivation::DontActivate);
				m_go_id_to_body_id[go_id] = box->GetID();
			}
			else if (bounding.type == BoundingType::Sphere)
			{
				JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(bounding.data[0]), JPH::RVec3(bounding.position[0], bounding.position[1], bounding.position[2]), 
					JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
				JPH::BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
				m_go_id_to_body_id[go_id] = sphere_id;
			}
		}*/

		m_physics_system->OptimizeBroadPhase();

	}
	PhysicsManager::~PhysicsManager()
	{
	}
	void PhysicsManager::tick()
	{
		float dt = Time::getInstance().getDeltaTime();
		m_physics_system->Update(dt, 1, m_temp_allocator, m_job_system);
		JPH::BodyInterface& body_interface = m_physics_system->GetBodyInterface();
		JPH::RVec3 position = body_interface.GetCenterOfMassPosition(m_go_id_to_body_id[1]);
		//m_scene.mesh_transform_list[1].position = { position.GetX(), position.GetY(), position.GetZ() };
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
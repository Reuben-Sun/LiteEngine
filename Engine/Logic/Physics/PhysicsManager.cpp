#include "PhysicsManager.h"
#include <cstdarg>
#include "Core/Time/Time.h"

namespace ToolEngine
{
	PhysicsManager::PhysicsManager(LogicScene& scene): m_scene(scene)
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
		
		createPhysicsScene();

		m_physics_system->OptimizeBroadPhase();

	}
	PhysicsManager::~PhysicsManager()
	{
	}
	void PhysicsManager::tick()
	{
		OPTICK_PUSH("Physics Update");
		float dt = Time::getInstance().getDeltaTime();
		m_physics_system->Update(dt, 1, m_temp_allocator, m_job_system);
		OPTICK_POP();
		OPTICK_PUSH("Modify entity transform");
		JPH::BodyInterface& body_interface = m_physics_system->GetBodyInterface();
		auto view = m_scene.scene_context.view<TransformComponent, const PhysicsComponent, const BoundingComponent>();
		for (auto entity : view)
		{
			auto& transform_component = view.get<TransformComponent>(entity);
			auto& physics_component = view.get<PhysicsComponent>(entity);
			auto& bounding_component = view.get<BoundingComponent>(entity);
			Bounding bounding = bounding_component.bounding;
			JPH::RVec3 position = body_interface.GetCenterOfMassPosition(physics_component.body_id);
			glm::vec3 new_position(position.GetX(), position.GetY(), position.GetZ());
			transform_component.transform.position = new_position - bounding.position;
		}
		OPTICK_POP();
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

	void PhysicsManager::createPhysicsScene()
	{
		OPTICK_EVENT();
		JPH::BodyInterface& body_interface = m_physics_system->GetBodyInterface();
		auto view = m_scene.scene_context.view<const TransformComponent, const BoundingComponent>();
		for (auto entity : view)
		{
			auto& transform_component = view.get<TransformComponent>(entity);
			auto& bounding_component = view.get<BoundingComponent>(entity);
			Transform transform = transform_component.transform;
			Bounding bounding = bounding_component.bounding;
			PhysicsComponent physics_component;
			if (bounding.type == BoundingType::Box)
			{
				JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(bounding.data[0], bounding.data[1], bounding.data[2]));
				JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
				JPH::ShapeRefC floor_shape = floor_shape_result.Get();
				JPH::RVec3 floor_position(transform.position[0] + bounding.position[0], 
					transform.position[1] + bounding.position[1], transform.position[2] + bounding.position[2]);

				JPH::BodyCreationSettings floor_settings(floor_shape, floor_position,
					JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);
				JPH::Body* box = body_interface.CreateBody(floor_settings);
				body_interface.AddBody(box->GetID(), JPH::EActivation::DontActivate);
				physics_component.body_id = box->GetID();
			}
			else if (bounding.type == BoundingType::Sphere)
			{
				JPH::RVec3 sphere_position(transform.position[0] + bounding.position[0],
					transform.position[1] + bounding.position[1], transform.position[2] + bounding.position[2]);
				JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(bounding.data[0]), sphere_position,
					JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
				JPH::BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
				physics_component.body_id = sphere_id;
			}
			m_scene.scene_context.emplace<PhysicsComponent>(entity, physics_component);
		}
	}
	
}
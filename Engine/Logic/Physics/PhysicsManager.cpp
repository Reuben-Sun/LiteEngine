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
		OPTICK_EVENT();
		float fixed_delta_time = getFixedDeltaTime();
		m_accumulator += Time::getInstance().getDeltaTime();
		if (m_accumulator > fixed_delta_time)
		{
			// HACK: A very long tick is only allowed to run once
			while (m_accumulator > fixed_delta_time)
			{
				m_accumulator -= fixed_delta_time;
			}
			fixedTick();
		}
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

	void PhysicsManager::fixedTick()
	{
		OPTICK_PUSH("Physics Update");
		float dt = getFixedDeltaTime();
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

	void PhysicsManager::pickup()
	{
		auto view = m_scene.scene_context.view<CameraComponent>();
		auto& camera = view.get<CameraComponent>(view.front()).camera;
		auto ui_view = m_scene.scene_context.view<UIInfoComponent>();
		auto& ui_info = ui_view.get<UIInfoComponent>(ui_view.front());
		if (ui_info.inSceneBounding())
		{
			LOG_INFO("Pickup");
			glm::vec3 begin = camera.transform.position;
			glm::vec2 uv = ui_info.getScreenUV();
			float ndc_x = 2.0f * uv.x - 1.0f;
			float ndc_y = 1.0f - 2.0f * uv.y;
			glm::mat inv_projection_mat = glm::inverse(camera.getProjectionMatrix());
			glm::mat inv_view_mat = glm::inverse(camera.getViewMatrix());
			
			glm::vec3 target = inv_view_mat * glm::vec4(ndc_x, ndc_y, camera.near_plane, 0.0f);
			//glm::vec3 target = inv_projection_mat * glm::vec4(ndc_x, ndc_y, -1.0, 1.0f);
			//target = inv_view_mat * glm::vec4(target.x, target.y, -1.0f, 0.0f);
			LOG_INFO("Camera position x:{0} y:{1} z:{2}", begin.x, begin.y, begin.z);
			LOG_INFO("Hit target x:{0} y:{1} z:{2}", target.x, target.y, target.z);
			glm::vec3 dir = target - begin;
			JPH::BodyInterface& body_interface = m_physics_system->GetBodyInterface();
			JPH::AllHitCollisionCollector<JPH::RayCastBodyCollector> collector;
			const JPH::BroadPhaseQuery& bp = m_physics_system->GetBroadPhaseQuery();
			JPH::RayCast ray_cast(JPH::RVec3(begin.x, begin.y, begin.z), JPH::RVec3(dir.x, dir.y, dir.z));
			bp.CastRay(ray_cast, collector);
			size_t nums = collector.mHits.size();
			JPH::BroadPhaseCastResult* results = collector.mHits.data();
			for (int i = 0; i < nums; i++)
			{
				glm::vec3 hit_pos = begin + dir * results[i].mFraction;
				
				LOG_INFO("Hit position: {0}, {1}, {2}", hit_pos.x, hit_pos.y, hit_pos.z);
				ui_info.test_pos = hit_pos;
				break;
			}
		}
		
	}
}
#include "PhysicsManager.h"

namespace ToolEngine
{
	PhysicsManager::PhysicsManager(RenderScene& scene): m_scene(scene)
	{
		//JPH::Factory::sInstance = new JPH::Factory();
		//JPH::RegisterTypes();

		//m_physics_system = new JPH::PhysicsSystem();
		//m_jolt_broad_phase_layer_interface = new BPLayerInterfaceImpl();
	}
	PhysicsManager::~PhysicsManager()
	{
	}
	void PhysicsManager::tick()
	{
	}
}
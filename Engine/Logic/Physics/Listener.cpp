#include "Listener.h"

namespace ToolEngine
{
	JPH::ValidateResult MyContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
	{
		LOG_INFO("Contact validate callback");
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;;
	}
	void MyContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		LOG_INFO("A contact was added");
	}
	void MyContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		LOG_INFO("A contact was persisted");
	}
	void MyContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
	{
		LOG_INFO("A contact was removed");
	}
	void MyBodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData)
	{
		LOG_INFO("A body got activated");
	}
	void MyBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData)
	{
		LOG_INFO("A body went to sleep");
	}
}
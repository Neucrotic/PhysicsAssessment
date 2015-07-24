#pragma once
#include "glm\common.hpp"
#include <vector>

class PhysicsObject;
class RigidBody;
class Box;
class Sphere;
class Plane;

class PhysicsScene
{
	const int NUMBERSHAPE = 4;

public:

	PhysicsScene();
	PhysicsScene(glm::vec2 _gravity);
	PhysicsScene(glm::vec2 _gravity, float _timeStep);
	~PhysicsScene();

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

	void Update();
	void DebugScene();
	void RenderGizmos();
	
	void AddActor(PhysicsObject* _actor);
	void RemoveActor(PhysicsObject* _actor);

	void AddSphere(Sphere*& _sphere, glm::vec3 _position, glm::vec3 _velocity, float _mass, float _radius);
	void AddAABB(Box*& _box, glm::vec3 _position, glm::vec3 _velocity, float _mass, glm::vec3 _extents);
	void AddPlane(Plane** _plane, glm::vec3 _normal, float _distToOrigin);

	void CheckForCollisions();
	
	//Specific Collision Functions
	static bool PlaneToPlane(PhysicsObject* _plane, PhysicsObject* _plane2);
	static bool PlaneToSphere(PhysicsObject* _plane, PhysicsObject* _sphere);
	static bool PlaneToBox(PhysicsObject* _plane, PhysicsObject* _box);
	static bool PlaneToJoint(PhysicsObject* _plane, PhysicsObject* _joint);
	
	static bool SphereToPlane(PhysicsObject* _sphere, PhysicsObject* _plane);
	static bool SphereToSphere(PhysicsObject* _sphereA, PhysicsObject* _sphereB);
	static bool SphereToBox(PhysicsObject* _sphere, PhysicsObject* _box);
	static bool SphereToJoint(PhysicsObject* _sphere, PhysicsObject* _joint);

	static bool BoxToPlane(PhysicsObject* _box, PhysicsObject* _plane);
	static bool BoxToSphere(PhysicsObject* _box, PhysicsObject* _sphere);
	static bool BoxToBox(PhysicsObject* _boxA, PhysicsObject* _boxB);
	static bool BoxToJoint(PhysicsObject* _box, PhysicsObject* _joint);

	static bool JointToPlane(PhysicsObject* _joint, PhysicsObject* _plane);
	static bool JointToBox(PhysicsObject* _joint, PhysicsObject* _box);
	static bool JointToSphere(PhysicsObject* _joint, PhysicsObject* _sphere);
	static bool JointToJoint(PhysicsObject* _joint, PhysicsObject* _joint2);

private:

	static glm::vec3 GetSmallestAxis(glm::vec3 &_vec);
	static glm::vec2 GetAABBMoveRatio(Box* _boxA, Box* _boxB);

	static glm::vec3 GetCollisionRestitutionVector(RigidBody* _objA, RigidBody* _objB);
	static void ResolveCollisions(glm::vec3 _crv, RigidBody* _obj1, RigidBody* _obj2);

};
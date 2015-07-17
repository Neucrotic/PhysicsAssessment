#pragma once
#include "glm\common.hpp"
#include <vector>

class PhysicsObject;

class PhysicsScene
{
	const int NUMBERSHAPE = 3;

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

	void CheckForCollisions();

	//Specific Collision Functions
	static bool PlaneToPlane(PhysicsObject* _planeA, PhysicsObject* _planeB);
	static bool PlaneToSphere(PhysicsObject* _plane, PhysicsObject* _sphere);
	static bool PlaneToBox(PhysicsObject* _plane, PhysicsObject* _box);
	
	static bool SphereToPlane(PhysicsObject* _sphere, PhysicsObject* _plane);
	static bool SphereToSphere(PhysicsObject* _sphereA, PhysicsObject* _sphereB);
	static bool SphereToBox(PhysicsObject* _sphere, PhysicsObject* _box);
	 
	static bool BoxToPlane(PhysicsObject* _box, PhysicsObject* _plane);
	static bool BoxToSphere(PhysicsObject* _box, PhysicsObject* _sphere);
	static bool BoxToBox(PhysicsObject* _boxA, PhysicsObject* _boxB);

private:
};
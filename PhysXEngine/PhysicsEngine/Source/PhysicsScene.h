#pragma once
#include "glm\common.hpp"
#include <vector>

class PhysicsObject;

class PhysicsScene
{
public:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> actors;

	void AddActor(PhysicsObject* _actor);
	void RemoveActor(PhysicsObject* _actor);
	void Update();
	void DebugScene();
	void RenderGizmos();
};
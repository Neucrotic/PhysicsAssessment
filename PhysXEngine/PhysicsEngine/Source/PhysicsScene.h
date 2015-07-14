#pragma once
#include "glm\common.hpp"
#include <vector>

class PhysicsObject;

class PhysicsScene
{
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
};
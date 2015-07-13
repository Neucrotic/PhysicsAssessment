#pragma once
#include "PhysicsObject.h"


class RigidBody : public PhyicsObject
{
public:

	RigidBody(glm::vec2 _position, glm::vec2 _velocity, float _rotation, float _mass);

	glm::vec2 m_position;
	glm::vec2 m_velocity;

	float m_mass;
	float m_rotation2D;

	virtual void Update(glm::vec2 _gravity, float _timeStep);
	virtual void Debug();

	void ApplyForce(glm::vec2 _force);
	void ApplyForceToActor(RigidBody* _actor, glm::vec2 _force);

private:



};
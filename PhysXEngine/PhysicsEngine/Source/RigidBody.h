#pragma once
#include "PhysicsObject.h"


class RigidBody : public PhyicsObject
{
public:

	RigidBody(glm::vec3 _position, glm::vec3 _velocity, float _rotation, float _mass);
	RigidBody(glm::vec3 _position, glm::vec3 _velocity, float _mass);

	glm::vec3 m_position;
	glm::vec3 m_velocity;

	float m_mass;
	float m_rotation2D;

	virtual void Update(glm::vec2 _gravity, float _timeStep);
	virtual void Debug();

	void ApplyForce(glm::vec3 _force);
	void ApplyForceToActor(RigidBody* _actor, glm::vec3 _force);

private:



};
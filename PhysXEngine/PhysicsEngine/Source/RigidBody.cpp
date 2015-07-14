#include "RigidBody.h"

RigidBody::RigidBody(glm::vec3 _position, glm::vec3 _velocity, float _rotation, float _mass)
{
	m_position = _position;
	m_velocity = _velocity;
	m_rotation2D = _rotation;
	m_mass = _mass;
}

RigidBody::RigidBody(glm::vec3 _position, glm::vec3 _velocity, float _mass)
{
	m_position = _position;
	m_velocity = _velocity;
	m_mass = _mass;
}

void RigidBody::Update(glm::vec2 _gravity, float _timeStep)
{
	_gravity *= 1;
	_timeStep += 0.001f;
}

void RigidBody::Debug()
{

}

void RigidBody::ApplyForce(glm::vec3 _force)
{

}

void RigidBody::ApplyForceToActor(RigidBody* _actor, glm::vec3 _force)
{

}
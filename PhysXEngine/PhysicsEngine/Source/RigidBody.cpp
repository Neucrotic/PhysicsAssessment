#include "RigidBody.h"

RigidBody::RigidBody(glm::vec3 _position, glm::vec3 _velocity, float _rotation, float _mass, bool _isStatic)
{
	m_position = _position;
	m_velocity = _velocity;
	m_rotation2D = _rotation;
	m_mass = _mass;
	m_isStatic = _isStatic;
}

RigidBody::RigidBody(glm::vec3 _position, glm::vec3 _velocity, float _mass, bool _isStatic)
{
	m_position = _position;
	m_velocity = _velocity;
	m_mass = _mass;
	m_isStatic = _isStatic;
}

void RigidBody::Update(glm::vec2 _gravity, float _deltaTime)
{
	_gravity *= 1;
	_deltaTime += 0.001f;
	
	ApplyForce(glm::vec3(_gravity, 0));
	m_position += m_velocity * _deltaTime;
}

void RigidBody::Debug()
{

}

void RigidBody::ApplyForce(glm::vec3 _force)
{
	glm::vec3 acceleration(_force / m_mass);

	m_velocity += acceleration;
}

void RigidBody::ApplyForceToActor(RigidBody* _actor, glm::vec3 _force)
{
	
}
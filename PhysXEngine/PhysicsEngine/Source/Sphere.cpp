#include "Sphere.h"
#include "Giz_core\Gizmos.h"

Sphere::Sphere(glm::vec3 _position, glm::vec3 _velocity, float _mass, float _radius, glm::vec4 _colour, bool _isStatic)
	: RigidBody(_position, _velocity, _mass, _isStatic)
{
	m_position = _position;
	m_velocity = _velocity;
	m_mass = _mass;
	m_radius = _radius;

	m_colour = _colour;

	m_shapeID = SPHERE;
}

void Sphere::MakeGizmo()
{
	Gizmos::addSphere(m_position, m_radius, 10, 10, m_colour);
}
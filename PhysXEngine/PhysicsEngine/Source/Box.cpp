#include "Box.h"
#include "Giz_core\Gizmos.h"


Box::Box(glm::vec3 _position, glm::vec3 _velocity, float _mass, glm::vec3 _extents, glm::vec4 _colour, bool _isStatic) 
	: RigidBody(_position, _velocity, _mass, _isStatic)
{
	m_position = _position;
	m_velocity = _velocity;
	m_mass = _mass;
	m_extents = _extents;
	m_colour = _colour;
	m_isStatic = _isStatic;

	m_shapeID = ShapeType::BOX;
}

void Box::MakeGizmo()
{
	Gizmos::addAABBFilled(m_position, m_extents, m_colour, &m_transform);
}

void Box::SetMinMaxExtents()
{
	m_min = m_position - m_extents;
	m_max = m_position + m_extents;
}
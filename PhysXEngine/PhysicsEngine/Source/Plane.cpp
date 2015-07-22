#include "Plane.h"
#include "Giz_core\Gizmos.h"

Plane::Plane()
{
	m_normal = glm::vec3(0, 1, 0);
	m_distanceFromOrigin = 0;

	m_shapeID = PLANE;
}

Plane::Plane(glm::vec3 _normal, float _distanceToOrigin)
{
	m_normal = _normal;
	m_distanceFromOrigin = _distanceToOrigin;

	m_shapeID = PLANE;
}

void Plane::Update(glm::vec2 _gravity, float _deltaTime)
{

}

void Plane::Debug()
{

}

void Plane::MakeGizmo()
{
	//float lineSegmentLength = 200;
	//glm::vec4 colour(1, 1, 1, 1);
	//
	//glm::vec3 centrePoint = m_normal * m_distanceFromOrigin;
	//glm::vec3 parallel = glm::vec3(m_normal.y, -m_normal.x, 0);
	//
	//glm::vec3 start = centrePoint + (parallel * lineSegmentLength);
	//glm::vec3 end = centrePoint - (parallel * lineSegmentLength);
	//
	//Gizmos::addLine(start, end, colour);

	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(500, 0.001, 500), glm::vec4(1, 1, 1, 1));
}
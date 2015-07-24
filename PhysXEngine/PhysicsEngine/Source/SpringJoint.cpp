#include "SpringJoint.h"
#include "Giz_core\Gizmos.h"
#include "RigidBody.h"

SpringJoint::SpringJoint(RigidBody* _connection1, RigidBody* _connection2, float _springConstant, float _damping)
{
	m_connections[0] = _connection1;
	m_connections[1] = _connection2;
	m_springConstant = _springConstant;
	m_damping = _damping;

	m_restLength = glm::length(m_connections[0]->m_position - m_connections[1]->m_position);

	m_shapeID = JOINT;
}

void SpringJoint::Update(glm::vec2 _gravity, float _dt)
{
	glm::vec3 D = m_connections[0]->m_position - m_connections[1]->m_position;
	float X = glm::length(D) - m_restLength;

	glm::vec3 F = (-m_springConstant) * (X * glm::normalize(D));

	for (int i = 0; i < 2; i++)
	{
		if (!m_connections[i]->m_isStatic)
		{
			//damping before apply force
			//glm::vec3 damp = m_damping * m_connections[i]->m_velocity;
			//F -= damp;

			m_connections[i]->m_velocity += F;
		}
	}
}

void SpringJoint::Debug()
{

}

void SpringJoint::MakeGizmo()
{
	Gizmos::addLine(m_connections[0]->m_position, m_connections[1]->m_position, glm::vec4(1, 1, 1, 1));
}
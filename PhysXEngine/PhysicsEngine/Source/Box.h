#pragma once 
#include "RigidBody.h"

class Box : public RigidBody
{
public:

	Box(glm::vec3 _position, glm::vec3 _velocity, float _mass, glm::vec3 _extents, glm::vec4 _colour);

	glm::vec3 m_extents;
	glm::mat4 m_transform;

	glm::vec3 m_min;
	glm::vec3 m_max;

	virtual void MakeGizmo();
	void SetMinMaxExtents();

private:

	glm::vec4 m_colour;

};
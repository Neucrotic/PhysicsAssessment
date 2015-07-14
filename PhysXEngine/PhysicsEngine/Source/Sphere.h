#pragma once 
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:

	Sphere(glm::vec3 _position, glm::vec3 _velocity, float _mass, float _radius, glm::vec4 _colour);

	float m_radius;

	virtual void MakeGizmo();

private:

	glm::vec4 m_colour;

};
#pragma once 
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:

	Sphere(glm::vec2 _position, glm::vec2 _velocity, float _mass, float _radius, glm::vec4 _colour);

	float m_radius;

	virtual void MakeGozmo();

private:

	

};
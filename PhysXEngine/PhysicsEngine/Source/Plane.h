#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:

	Plane();
	Plane(glm::vec3 _normal, float _distanceToOrigin);

	glm::vec3 m_normal;
	float m_distanceFromOrigin;

	virtual void Update(glm::vec2 _gravity, float _deltaTime);
	virtual void Debug();
	virtual void MakeGizmo();

private:
};
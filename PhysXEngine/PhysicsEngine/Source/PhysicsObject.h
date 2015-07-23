#pragma once
#include "glm/common.hpp"
#include "glm\mat4x4.hpp"

class PhysicsObject
{
public:

	enum ShapeType
	{
		PLANE,
		SPHERE,
		BOX
	};

	ShapeType m_shapeID;

	virtual void Update(glm::vec2 _gravity, float _deltaTime) = 0;
	virtual void Debug() = 0; //use to print debug information to the console
	virtual void MakeGizmo() = 0;
	void ResetPosition();

private:
};
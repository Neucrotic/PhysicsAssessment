#pragma once
#include "glm/common.hpp"

class PhyicsObject
{

	enum ShapeType
	{
		PLANE = 0,
		SPHERE = 1,
		BOX = 2
	};

public:

	ShapeType m_shapeID;

	virtual void Update(glm::vec2 _gravity, float _timeStep) = 0;
	virtual void Debug() = 0; //use to print debug information to the console
	virtual void MakeGizmo() = 0;
	void ResetPosition();

private:



};
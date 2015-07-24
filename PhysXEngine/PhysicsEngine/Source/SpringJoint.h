#pragma once
#include "PhysicsObject.h"

class RigidBody;

class SpringJoint : public PhysicsObject
{
public:

	SpringJoint(RigidBody* _connection1, RigidBody* _connection2, float _springConstant, float _damping);

private:

	virtual void Update(glm::vec2 _gravity, float _dt);
	virtual void Debug();
	virtual void MakeGizmo();

	RigidBody* m_connections[2];

	float m_damping;
	float m_restLength;
	float m_springConstant;

};
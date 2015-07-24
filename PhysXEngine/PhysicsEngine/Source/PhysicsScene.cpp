#include "PhysicsScene.h"
#include "glm\glm.hpp"
//#include <cmath>

#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "SpringJoint.h"

PhysicsScene::PhysicsScene()
{

}

PhysicsScene::PhysicsScene(glm::vec2 _gravity)
{
	m_gravity = _gravity;
}

PhysicsScene::PhysicsScene(glm::vec2 _gravity, float _timeStep)
{
	m_gravity = _gravity;
	m_timeStep = _timeStep;
}

PhysicsScene::~PhysicsScene()
{
	m_actors.clear();
}

void PhysicsScene::AddActor(PhysicsObject* _actor)
{
	m_actors.push_back(_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* _actor)
{
	m_actors.erase(m_actors.begin(), m_actors.end());
}

void PhysicsScene::AddSphere(Sphere*& _sphere, glm::vec3 _position, glm::vec3 _velocity, float _mass, float _radius)
{
	_sphere = new Sphere(_position, _velocity, _mass, _radius, glm::vec4(1, 0, 0, 1), false);

	AddActor(_sphere);
}

void PhysicsScene::AddAABB(Box*& _box, glm::vec3 _position, glm::vec3 _velocity, float _mass, glm::vec3 _extents)
{
	_box = new Box(_position, _velocity, _mass, _extents, glm::vec4(0, 0, 1, 1), false);

	AddActor(_box);
}

void PhysicsScene::AddPlane(Plane** _plane, glm::vec3 _normal, float _distToOrigin)
{
	*_plane = new Plane(_normal, _distToOrigin);

	AddActor(*_plane);
}

void PhysicsScene::Update()
{
	for (auto actor : m_actors)
	{
		if (!actor->m_isStatic || (actor->m_isStatic && actor->m_shapeID == 3))
		{
			actor->Update(m_gravity, m_timeStep);
		}
	}
}

void PhysicsScene::DebugScene()
{

}

void PhysicsScene::RenderGizmos()
{
	for (auto& actor : m_actors)
	{
		actor->MakeGizmo();
	}
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::PlaneToPlane, PhysicsScene::PlaneToSphere, PhysicsScene::PlaneToBox, PhysicsScene::PlaneToJoint,
	PhysicsScene::SphereToPlane, PhysicsScene::SphereToSphere, PhysicsScene::SphereToBox, PhysicsScene::SphereToJoint,
	PhysicsScene::BoxToPlane, PhysicsScene::BoxToSphere, PhysicsScene::BoxToBox, PhysicsScene::BoxToJoint,
	PhysicsScene::JointToPlane, PhysicsScene::JointToBox, PhysicsScene::JointToSphere, PhysicsScene::JointToJoint
};

void PhysicsScene::CheckForCollisions()
{
	
	int actorCount = m_actors.size();

	//checking collisions against all objects but this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];

			int meshID1 = object1->m_shapeID;
			int meshID2 = object2->m_shapeID;

			//using function pointers
			int functionIndex = (meshID1 * NUMBERSHAPE) + meshID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIndex];

			if (collisionFunctionPtr != NULL)
			{
				collisionFunctionPtr(object1, object2);
			}
		}
	}

}

//PLANE
bool PhysicsScene::PlaneToPlane(PhysicsObject* _plane, PhysicsObject* _plane2)
{
	return false;
}

bool PhysicsScene::PlaneToSphere(PhysicsObject* _plane, PhysicsObject* _sphere)
{
	return SphereToPlane(_sphere, _plane);

	return false;
}

bool PhysicsScene::PlaneToBox(PhysicsObject* _plane, PhysicsObject* _box)
{
	BoxToPlane(_box, _plane);

	return false;
}

bool PhysicsScene::PlaneToJoint(PhysicsObject* _plane, PhysicsObject* _joint)
{
	return false;
}
//SPHERE
bool PhysicsScene::SphereToPlane(PhysicsObject* _sphere, PhysicsObject* _plane)
{
	Sphere* sphere = dynamic_cast<Sphere*>(_sphere);
	Plane* plane = dynamic_cast<Plane*>(_plane);

	glm::vec3 collisionNormal = plane->m_normal;
	float sphereToPlane = glm::dot(sphere->m_position, plane->m_normal) - plane->m_distanceFromOrigin;

	//if behind the plane, flip the normal
	//if (sphereToPlane < 0)
	//{
	//	collisionNormal *= -1;
	//	sphereToPlane *= -1;
	//}

	float intersection = sphere->m_radius - sphereToPlane;

	if (intersection > 0)
	{
		sphere->m_velocity = glm::vec3(0);
		sphere->m_position += intersection * collisionNormal;
		return true;
	}

	return false;
}

bool PhysicsScene::SphereToSphere(PhysicsObject* _sphereA, PhysicsObject* _sphereB)
{
	Sphere* sphereA = dynamic_cast<Sphere*>(_sphereA);
	Sphere* sphereB = dynamic_cast<Sphere*>(_sphereB);

	if (sphereA != NULL && sphereB != NULL)
	{
		glm::vec3 collisionNormal = sphereA->m_position - sphereB->m_position;
		float distance = glm::length(collisionNormal);
		collisionNormal = glm::normalize(collisionNormal);

		if (distance < (sphereA->m_radius + sphereB->m_radius))
		{
			sphereA->m_velocity = glm::vec3(0);
			sphereB->m_velocity = glm::vec3(0);

			float intersecDist = (sphereA->m_radius + sphereB->m_radius) - distance;

			ResolveCollisions(intersecDist * collisionNormal, sphereA, sphereB);


			return true;
		}		
	}

	return false;
}

bool PhysicsScene::SphereToBox(PhysicsObject* _sphere, PhysicsObject* _box)
{
	BoxToSphere(_box, _sphere);

	return false;
}

bool PhysicsScene::SphereToJoint(PhysicsObject* _sphere, PhysicsObject* _joint)
{
	return false;
}
//BOX
bool PhysicsScene::BoxToPlane(PhysicsObject* _box, PhysicsObject* _plane)
{
	Box* box = dynamic_cast<Box*>(_box);
	Plane* plane = dynamic_cast<Plane*>(_plane);

	float halfExtents = box->m_extents.x / 2;

	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 point3;
	glm::vec3 point4;
	glm::vec3 point5;
	glm::vec3 point6;
	glm::vec3 point7;
	glm::vec3 point8;

#pragma region initialising_points

	point1.x = box->m_position.x + halfExtents;
	point1.y = box->m_position.y + halfExtents;
	point1.z = box->m_position.z + halfExtents;

	point2.x = box->m_position.x + halfExtents;
	point2.y = box->m_position.y + halfExtents;
	point2.z = box->m_position.z - halfExtents;

	point3.x = box->m_position.x + halfExtents;
	point3.y = box->m_position.y - halfExtents;
	point3.z = box->m_position.z + halfExtents;

	point4.x = box->m_position.x + halfExtents;
	point4.y = box->m_position.y - halfExtents;
	point4.z = box->m_position.z - halfExtents;

	point5.x = box->m_position.x - halfExtents;
	point5.y = box->m_position.y - halfExtents;
	point5.z = box->m_position.z + halfExtents;

	point6.x = box->m_position.x - halfExtents;
	point6.y = box->m_position.y - halfExtents;
	point6.z = box->m_position.z - halfExtents;

	point7.x = box->m_position.x - halfExtents;
	point7.y = box->m_position.y + halfExtents;
	point7.z = box->m_position.z + halfExtents;

	point8.x = box->m_position.x - halfExtents;
	point8.y = box->m_position.y + halfExtents;
	point8.z = box->m_position.z - halfExtents;
#pragma endregion



	return false;
}

bool PhysicsScene::BoxToSphere(PhysicsObject* _box, PhysicsObject* _sphere)
{


	return false;
}

bool PhysicsScene::BoxToBox(PhysicsObject* _boxA, PhysicsObject* _boxB)
{
	Box* box1 = dynamic_cast<Box*>(_boxA);
	Box* box2 = dynamic_cast<Box*>(_boxB);

	glm::vec3 intersectionPoint;

	if (box1 != NULL && box2 != NULL)
	{
		box1->SetMinMaxExtents();
		box2->SetMinMaxExtents();

		if (box1->m_max.x > box2->m_min.x && box1->m_min.x < box2->m_max.x)
		{
			float intersectX1 = box1->m_max.x - box2->m_min.x;
			float intersectX2 = box2->m_max.x - box1->m_min.x;
			
			if (glm::abs(intersectX1) < glm::abs(intersectX2))
			{
				intersectionPoint.x = intersectX1;
			}
			else
			{
				intersectionPoint.x = intersectX2;
			}
			
			if (box1->m_min.y < box2->m_max.y && box1->m_max.y > box2->m_min.y)
			{
				float intersectY1 = box2->m_max.y - box1->m_min.y;
				float intersectY2 = box1->m_max.y - box2->m_min.y;

				if (glm::abs(intersectY1) < glm::abs(intersectY2))
				{
					intersectionPoint.y = intersectY1;
				}
				else
				{
					intersectionPoint.y = intersectY2;
				}

				if (box1->m_min.z < box2->m_max.z && box1->m_max.z > box2->m_min.z)
				{
					float intersectZ1 = box2->m_max.z - box1->m_min.z;
					float intersectZ2 = box1->m_max.z - box2->m_min.z;

					if (glm::abs(intersectZ1) < glm::abs(intersectZ2))
					{
						intersectionPoint.z = intersectZ1;
					}
					else
					{
						intersectionPoint.z = intersectZ2;
					}

					//calculating CRV
					glm::vec3 colNorm = glm::normalize((box2->m_position - box1->m_position));
					float intersectDepth = (box1->m_extents.x + box2->m_extents.x) - glm::length(box2->m_position - box1->m_position);

					glm::vec3 CRV(colNorm * intersectDepth);

					//apply bounceBack
					ResolveCollisions(CRV, box1, box2);

					box1->m_velocity = glm::vec3(0);
					box2->m_velocity = glm::vec3(0);

					return true;
				}
			}
		}		
	}		

	return false;
}

bool PhysicsScene::BoxToJoint(PhysicsObject* _box, PhysicsObject* _joint)
{
	return false;
}
//JOINT
bool PhysicsScene::JointToPlane(PhysicsObject* _joint, PhysicsObject* _plane)
{
	return false;
}

bool PhysicsScene::JointToBox(PhysicsObject* _joint, PhysicsObject* _box)
{
	return false;
}

bool PhysicsScene::JointToSphere(PhysicsObject* _joint, PhysicsObject* _sphere)
{
	return false;
}

bool PhysicsScene::JointToJoint(PhysicsObject* _joint, PhysicsObject* _joint2)
{
	return false;
}

glm::vec3 PhysicsScene::GetSmallestAxis(glm::vec3 &_vec)
{
	if (_vec.x < _vec.y)
	{
		if (_vec.x < _vec.z)
		{
			_vec.y = 0; 
			_vec.z = 0;
			return _vec;
		}
		else
		{
			_vec.x = 0;
			_vec.y = 0;
			return _vec;
		}
	}
	else
	{
		if (_vec.y < _vec.z)
		{
			_vec.x = 0;
			_vec.z = 0;
			return _vec;
		}
		else
		{
			_vec.x = 0;
			_vec.y = 0;
			return _vec;
		}
	}
}

glm::vec2 PhysicsScene::GetAABBMoveRatio(Box* _boxX, Box* _boxY)
{
	float moveRatioA = 1 - (_boxX->m_mass / (_boxX->m_mass + _boxY->m_mass));
	float moveRatioB = 1 - (_boxY->m_mass / (_boxX->m_mass + _boxY->m_mass));

	return glm::vec2(moveRatioA, moveRatioB);
}

void PhysicsScene::ResolveCollisions(glm::vec3 _crv, RigidBody* _objA, RigidBody* _objB)
{
	float bounceA = _objA->m_mass / (_objA->m_mass + _objB->m_mass);
	float bounceB = _objB->m_mass / (_objA->m_mass + _objB->m_mass);

	_objA->m_position -= (_crv * bounceA);
	_objB->m_position += (_crv * bounceB);
}
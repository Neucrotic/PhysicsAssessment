#include "PhysicsScene.h"
#include "glm\glm.hpp"
#include <cmath>

#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"

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

void PhysicsScene::Update()
{
	for (auto actor : m_actors)
	{
		actor->Update(m_gravity, m_timeStep);
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
	PhysicsScene::PlaneToPlane,  PhysicsScene::PlaneToSphere,  PhysicsScene::PlaneToBox,
	PhysicsScene::SphereToPlane, PhysicsScene::SphereToSphere, PhysicsScene::SphereToBox,
	PhysicsScene::BoxToPlane,	 PhysicsScene::BoxToSphere,    PhysicsScene::BoxToBox

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
bool PhysicsScene::PlaneToPlane(PhysicsObject* _planeA, PhysicsObject* _planeB)
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

			float fIntersecDist = (sphereA->m_radius + sphereB->m_radius) - distance;

			sphereA->m_position += fIntersecDist * collisionNormal;
			sphereB->m_position -= fIntersecDist * collisionNormal;


			return true;
		}		
	}

	return false;
}

bool PhysicsScene::SphereToBox(PhysicsObject* _sphere, PhysicsObject* _box)
{


	return false;
}
//BOX
bool PhysicsScene::BoxToPlane(PhysicsObject* _box, PhysicsObject* _plane)
{


	return false;
}

bool PhysicsScene::BoxToSphere(PhysicsObject* _box, PhysicsObject* _sphere)
{


	return false;
}

bool PhysicsScene::BoxToBox(PhysicsObject* _boxA, PhysicsObject* _boxB)
{


	return false;
}
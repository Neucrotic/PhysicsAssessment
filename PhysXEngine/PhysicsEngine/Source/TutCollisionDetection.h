#pragma once

#include "App.h"

class Box;
class Sphere;
class Plane;
class PhysicsScene;

class TutCollisionDetection : public App
{
public:

	virtual void Startup();
	virtual void Shutdown();

	virtual bool Update(double _dt);
	virtual void Render();

private:

	PhysicsScene* m_physicsScene;

	Sphere* m_mySphereA;
	Sphere* m_mySphereB;

	Plane* m_myPlane;

	Box* m_myBoxA;
	Box* m_myBoxB;

};
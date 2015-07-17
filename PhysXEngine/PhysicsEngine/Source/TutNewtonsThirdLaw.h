#pragma once
#include "App.h"

class Sphere;
class PhysicsScene;

class TutNewtonsThirdLaw : public App
{
public:

	virtual void Startup();
	virtual void Shutdown();

	virtual bool Update(double _dt);
	virtual void Render();

private:

	PhysicsScene* m_physicsScene;

	Sphere* m_ballA;
	Sphere* m_ballB;

};
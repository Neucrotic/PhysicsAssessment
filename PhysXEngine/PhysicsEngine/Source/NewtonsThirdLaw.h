#pragma once
#include "App.h"

class Sphere;
class PhysicsScene;

class NewtonsThirdLaw : public App
{
public:

	virtual void Startup();
	virtual void Shutdown();

	virtual bool Update(double _dt);
	virtual void Render();

private:

	PhysicsScene* physicsScene;

	Sphere* ballA;
	Sphere* ballB;

};
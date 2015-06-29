#pragma once
#include "App.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd\PxVisualDebugger.h>

using namespace physx;

class PhysXApp : public App
{
public:

	void Startup();
	void Shutdown();

	bool Update(double _dt);
	void Render();

private:

	PxFoundation* g_PhysicsFoundation;
	PxPhysics* g_Physics;
	PxScene* g_PhysicsScene;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxMaterial* g_PhysicsMaterial;
	PxMaterial* g_boxMaterial;
	PxCooking* g_PhysicsCooker;

	//PhysX Functions
	void SetUpPhysX();
	void UpdatePhysX(double _dt);
	void SetUpVisualDebugger();

	//tutorial setup functions
	void SetupTutorial1();

};

class myAllocator : public physx::PxAllocatorCallback
{
public:

	virtual ~myAllocator() {}
	virtual void* allocate(size_t _size, const char* _typeName, const char* _fileName, int _line)
	{
		void* pointer = _aligned_malloc(_size, 16);
		return pointer;
	}
	virtual void deallocate(void* _ptr)
	{
		_aligned_free(_ptr);
	}
};
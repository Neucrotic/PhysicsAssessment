#pragma once
#include "App.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd\PxVisualDebugger.h>
#include <vector>

using namespace physx;

class PhysXApp : public App
{
public:

	void Startup();
	void Shutdown();

	bool Update(double _dt);
	void Render();

private:

	//List of scenes Actors
	std::vector<PxRigidBody*> physxActors;

	//PhysX specific
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

	void AddWidget(PxShape* _shape, PxRigidActor* _actor);
	void AddBox(PxShape* _shape, PxRigidActor* _actor);
	void AddSphere(PxShape* _shape, PxRigidActor* _actor);
	void AddSphere(PxShape* _shape, PxRigidActor* _actor, glm::vec3 _startPos);
	void AddCapsule(PxShape* _shape, PxRigidActor* _actor);

	void FireSphere(glm::mat4 _camTrans);
	void MovePlayerController(double _dt);

	//tutorial setup functions
	void SetupTutorial1();
	void SetUpTutRagdoll();
	void SetupTutController();

	//input
	float keyCD;

	//values for camera followX
	PxRigidDynamic* m_capsule;
	glm::vec3 m_cameraPos;
	glm::vec3 m_camOffset;
	//and object follow
	ControllerHitReport* m_myHitReport;
	float m_characterYVelocity;
	PxQuat m_characterRotation;
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

class ControllerHitReport : public PxUserControllerHitReport
{
public:
	ControllerHitReport() : PxUserControllerHitReport(){};

	PxVec3 m_playerContactNormal;

	virtual void OnShapeHit(const PxControllerShapeHit &_hit);

	PxVec3 GetPlayerContactNormal() { return m_playerContactNormal; }
	void ClearPlayerContactNormal() { m_playerContactNormal = PxVec3(0, 0, 0); }

	//functions NEED to be overloaded, not needed
	virtual void OnControllerHit(const PxControllersHit &_hit){};
	virtual void OnObstacleHit(const PxControllerObstacleHit &_hit){};
};
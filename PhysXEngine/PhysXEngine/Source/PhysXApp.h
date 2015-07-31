#pragma once
#include "App.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd\PxVisualDebugger.h>
#include <vector>

using namespace physx;

class ControllerHitReport;
class ParticleFluidEmitter;

class PhysXApp : public App
{
public:

	void Startup();
	void Shutdown();

	bool Update(double _dt);
	void Render();

private:

	//List of scenes Actors
	std::vector<PxRigidActor*> m_physxActors;

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

	//input
	float keyCD;

	//fluid simulation
	ParticleFluidEmitter* m_particleEmitter;

	//values for camera followX
	PxRigidDynamic* m_capsule;
	glm::vec3 m_cameraPos;
	glm::vec3 m_camOffset;
	//and object follow
	PxController* m_playerController;
	ControllerHitReport* m_myHitReport;
	float m_characterYVelocity;
	float m_characterRotation;
	float m_playerGravity;

	//PhysX Functions
	void SetUpPhysX();
	void UpdatePhysX(double _dt);
	void SetUpVisualDebugger();

	//rendering gizmos
	void AddWidget(PxShape* _shape, PxRigidActor* _actor);
	void AddBox(PxShape* _shape, PxRigidActor* _actor);
	void AddSphere(PxShape* _shape, PxRigidActor* _actor);
	void AddSphere(PxShape* _shape, PxRigidActor* _actor, glm::vec3 _startPos);
	void AddCapsule(PxShape* _shape, PxRigidActor* _actor);

	//'game physics'
	void FireSphere(glm::mat4 _camTrans);
	void MovePlayerController(double _dt);

	//tutorial setup functions
	void SetupTutorial1();
	void SetUpTutRagdoll();
	void SetupTutController();
	void SetupTutFluid();

	//helper functions
	glm::vec3 Vec3PhysXToGLM(PxVec3 &_vec);
	PxVec3 Vec3GLMToPhysX(glm::vec3 &_vec);
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

	void onShapeHit(const PxControllerShapeHit &_hit) override;

	PxVec3 GetPlayerContactNormal() { return m_playerContactNormal; }
	void ClearPlayerContactNormal() { m_playerContactNormal = PxVec3(0, 0, 0); }

	//functions NEED to be overloaded, not needed
	virtual void onControllerHit(const PxControllersHit &_hit) override {}
	virtual void onObstacleHit(const PxControllerObstacleHit &_hit) override {};
};
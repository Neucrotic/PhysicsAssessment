#pragma once
#include "App.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd\PxVisualDebugger.h>
#include <vector>

using namespace physx;

class ParticleFluidEmitter;
class PlayerController;

struct FilterGroup
{
	enum Enum
	{
		ePLAYER = (1 << 0),
		ePLATFORM = (1 << 1),
		eGROUND = (1 << 2)
	};
};

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
	PlayerController* m_player;
	PxRigidDynamic* m_capsule;
	glm::vec3 m_cameraPos;
	glm::vec3 m_camOffset;

	//PhysX Functions
	void SetUpPhysX();
	void UpdatePhysX(double _dt);
	void SetUpVisualDebugger();

	//Trigger helper functions
	void SetUpFiltering(PxRigidActor* _actor, PxU32 _filterGroup, PxU32 _filterMask);
	void SetShapeAsTrigger(PxRigidActor* _actor);

	//rendering gizmos
	void AddWidget(PxShape* _shape, PxRigidActor* _actor);
	void AddBox(PxShape* _shape, PxRigidActor* _actor);
	void AddSphere(PxShape* _shape, PxRigidActor* _actor);
	void AddSphere(PxShape* _shape, PxRigidActor* _actor, glm::vec3 _startPos);
	void AddCapsule(PxShape* _shape, PxRigidActor* _actor);

	//'game physics'
	void FireSphere(glm::mat4 _camTrans);

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

class myCollisionCallBack : public PxSimulationEventCallback
{
	virtual void onContact(const PxContactPairHeader& _pairHeader, const PxContactPair* _pairs, PxU32 _numPairs) override;
	virtual void onTrigger(PxTriggerPair* _pairs, PxU32 _numPairs) override;
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) override {};
	virtual void onWake(PxActor**, PxU32) override {};
	virtual void onSleep(PxActor**, PxU32) override {};
};
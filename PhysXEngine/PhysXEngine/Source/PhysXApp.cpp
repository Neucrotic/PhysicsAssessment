#include "PhysXApp.h"

void PhysXApp::Startup()
{


	SetUpPhysX();
	SetUpVisualDebugger();

	SetupTutorial1();
}

void PhysXApp::Shutdown()
{
	g_PhysicsScene->release();
	g_Physics->release();
	g_PhysicsFoundation->release();
}

bool PhysXApp::Update(double _dt)
{
	//m_camera->Update(_dt);
	UpdatePhysX(_dt);



	return true;
}

void PhysXApp::Render()
{

}

void PhysXApp::SetUpPhysX()
{
	PxAllocatorCallback* myCallback = new myAllocator();

	g_PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myCallback, gDefaultErrorCallback);
	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_PhysicsFoundation, PxTolerancesScale());

	PxInitExtensions(*g_Physics);

	//create physics material
	g_PhysicsMaterial = g_Physics->createMaterial(0.5f, 0.5f, 0.5f);

	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	g_PhysicsScene = g_Physics-> createScene(sceneDesc);
}

void PhysXApp::UpdatePhysX(double _dt)
{
	if (_dt <= 0)
	{
		return;
	}

	g_PhysicsScene->simulate((float)_dt);

	while (!g_PhysicsScene->fetchResults())
	{

	}
}

void PhysXApp::SetUpVisualDebugger()
{
	//check if PvdConnection manager is available on this platform
	if (g_Physics->getPvdConnectionManager() == NULL)
		return;

	//setup connection parameters
	//IP of the PC which is running PVD
	const char* pvd_host_ip = "127.0.0.1";

	//TCP port to connect to, where PVD is listening
	int port = 5425;

	//timeout in milliseconds to wait for PCD to respond, concoles and remote PCs have a higher timeout.
	unsigned int timeout = 100;

	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	//trying to connect PxVisualDebuggerExt
	auto theConnection = PxVisualDebuggerExt::createConnection(g_Physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
}

void PhysXApp::SetupTutorial1()
{
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), physx::PxQuat(PxHalfPi * 1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(), *g_PhysicsMaterial);

	//add the plane to the scene
	g_PhysicsScene->addActor(*plane);

	//add a box
	float density = 10;
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(0, 5, 0));
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*g_Physics, transform, box, *g_PhysicsMaterial, density);

	//add the box to the physX scene
	g_PhysicsScene->addActor(*dynamicActor);
}
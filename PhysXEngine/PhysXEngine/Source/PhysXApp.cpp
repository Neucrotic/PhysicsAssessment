#include "PhysXApp.h"
#include "Camera\MobileCamera.h"

void PhysXApp::Startup()
{
	MobileCamera* camera = new MobileCamera(50.0f, 0.1f);
	camera->SetInputWindow(window);
	camera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	camera->LookAt(glm::vec3(0, 1, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_camera = camera;

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
	m_camera->Update(_dt);
	UpdatePhysX(_dt);



	return true;
}

void PhysXApp::Render()
{
	Gizmos::addTransform(glm::mat4(1));
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

	for (auto actor : physxActors)
	{
		PxU32 numShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape*[numShapes];
		actor->getShapes(shapes, numShapes);

		while (numShapes--)
		{
			AddWidget(shapes[numShapes], actor);
		}
		delete[] shapes;
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

void PhysXApp::AddWidget(PxShape* _shape, PxRigidActor* _actor)
{
	PxGeometryType::Enum type = _shape->getGeometryType();

	switch (type)
	{
	case PxGeometryType::eBOX:
		AddBox(_shape, _actor);
		break;
	}
}

void PhysXApp::AddBox(PxShape* _shape, PxRigidActor* _actor)
{
	PxBoxGeometry geometry;
	float width = 1;
	float height = 1;
	float length = 1;

	bool status = _shape->getBoxGeometry(geometry);
	
	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;

		PxMat44 m(PxShapeExt::getGlobalPose(*_shape, *_actor));

		glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
					m.column1.x, m.column1.y, m.column1.z, m.column1.w,
					m.column2.x, m.column2.y, m.column2.z, m.column2.w,
					m.column3.x, m.column3.y, m.column3.z, m.column3.w);

		glm::vec3 position;

		position.x = m.getPosition().x;
		position.y = m.getPosition().y;
		position.z = m.getPosition().z;

		glm::vec3 extents = glm::vec3(width, height, length);
		glm::vec4 colour = glm::vec4(1, 0, 0, 1);

		if (_actor->getName() != NULL && strcmp(_actor->getName(), "Pickup1"))
		{
			colour = glm::vec4(0, 1, 0, 1);
		}

		Gizmos::addAABBFilled(position, extents, colour, &M);
	}
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
	physxActors.push_back(dynamicActor);
}
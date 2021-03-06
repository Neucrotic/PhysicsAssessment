#include <iostream>
#include "PhysXApp.h"
#include "Camera\MobileCamera.h"
#include "Ragdoll.h"
#include "RagdollData.h"
#include "ParticleEmitter.h";
#include "ParticleFluidEmitter.h";
#include "PlayerController.h"

PxFilterFlags myFilterShader(PxFilterObjectAttributes _attributes0, PxFilterData _filterData0, PxFilterObjectAttributes _attributes1, PxFilterData _filterData1, PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize)
{
	//let triggers through
	if (PxFilterObjectIsTrigger(_attributes0) || PxFilterObjectIsTrigger(_attributes1))
	{
		_pairFlags = PxPairFlag::eTRIGGER_DEFAULT;

		return PxFilterFlag::eDEFAULT;
	}

	//generate contacts for all that were not filtered above
	_pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	//trigger the contact callback for pair (A,B) 
	//where the filtermask of A contains the ID of B and vice versa
	if ((_filterData0.word0 & _filterData1.word1) && (_filterData1.word0 & _filterData0.word1))
	{
		_pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
	}

	return PxFilterFlag::eDEFAULT;
}

void PhysXApp::Startup()
{
	Camera* camera = new Camera();
	camera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	camera->LookAt(glm::vec3(0, 1, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_camera = camera;

	SetUpPhysX();
	SetUpVisualDebugger();

	//SetupTutorial1();
	//SetUpTutRagdoll();
	SetupTutController();
	//SetupTutFluid();

	keyCD = 0.15f;
}

void PhysXApp::Shutdown()
{
	g_PhysicsScene->release();
	g_Physics->release();
	g_PhysicsFoundation->release();
}

bool PhysXApp::Update(double _dt)
{
	UpdatePhysX(_dt);
	
	m_camera->Update(_dt);	
	m_player->MovePlayerController(_dt);

	m_camera->SetTransform(m_player->GetTransform());
	//m_camera->SetPosition(m_player->GetPosition());
	//m_camera->LookAt(lookAtPos, glm::vec3(0, 1, 0));

	//updating particles
	//if (m_particleEmitter)
	//{
	//	m_particleEmitter->update(_dt);
	//}

	if (keyCD <= 0)
	{
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			FireSphere(m_camera->GetTransform());
		}

		keyCD = 0.15f;
	}

	keyCD -= _dt;

	return true;
}

void PhysXApp::Render()
{
	Gizmos::addTransform(glm::mat4(1));

	//rendering particles
	//if (m_particleEmitter)
	//{
	//	m_particleEmitter->renderParticles();
	//}
}

void PhysXApp::SetUpPhysX()
{
	PxAllocatorCallback* myCallback = new myAllocator();

	g_PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myCallback, gDefaultErrorCallback);
	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_PhysicsFoundation, PxTolerancesScale());

	PxInitExtensions(*g_Physics);

	//create physics material
	g_PhysicsMaterial = g_Physics->createMaterial(0.5f, 0.5f, 0.5f);

	PxSimulationEventCallback* myCollisionCallback = new myCollisionCallBack();
	
	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &myFilterShader;
	sceneDesc.simulationEventCallback = myCollisionCallback;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	g_PhysicsScene = g_Physics->createScene(sceneDesc);

	
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

	for (auto actor : m_physxActors)
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

void PhysXApp::SetUpFiltering(PxRigidActor* _actor, PxU32 _filterGroup, PxU32 _filterMask)
{
	PxFilterData filterData;
	filterData.word0 = _filterGroup; // word0 = own ID
	filterData.word1 = _filterMask; // word1 = ID mask to filter pairs that trigger a contact callback

	const PxU32 numShapes = _actor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*) * numShapes, 16);

	_actor->getShapes(shapes, numShapes);

	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}

	_aligned_free(shapes);
}

void PhysXApp::SetShapeAsTrigger(PxRigidActor* _actor)
{
	PxRigidStatic* staticActor = _actor->is<PxRigidStatic>();
	assert(staticActor);

	const PxU32 numShapes = staticActor->getNbShapes();

	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*) * numShapes, 16);
	staticActor->getShapes(shapes, numShapes);

	for (PxU32 i = 0; i < numShapes; i++)
	{
		shapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
}

void PhysXApp::AddWidget(PxShape* _shape, PxRigidActor* _actor)
{
	PxGeometryType::Enum type = _shape->getGeometryType();

	switch (type)
	{
	case PxGeometryType::eBOX:
		AddBox(_shape, _actor);
		break;
	case PxGeometryType::eSPHERE:
		AddSphere(_shape, _actor);
		break;
	case PxGeometryType::eCAPSULE:
		AddCapsule(_shape, _actor);
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

		M = glm::transpose(M);

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

void PhysXApp::AddSphere(PxShape* _shape, PxRigidActor* _actor)
{
	PxSphereGeometry geometry;
	
	float radius = 1;

	bool status = _shape->getSphereGeometry(geometry);

	if (status)
	{
		radius = geometry.radius;
	}

	PxMat44 m(PxShapeExt::getGlobalPose(*_shape, *_actor));

	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
				m.column1.x, m.column1.y, m.column1.z, m.column1.w,
				m.column2.x, m.column2.y, m.column2.z, m.column2.w,
				m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	M = glm::transpose(M);

	glm::vec3 position;

	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec4 colour = glm::vec4(0, 0, 1, 1);

	Gizmos::addSphere(position, radius, (radius * 10), (radius * 10), colour, &M);
}

void PhysXApp::AddCapsule(PxShape* _shape, PxRigidActor* _actor)
{
	PxCapsuleGeometry geometry;

	float radius;
	float halfHeight;

	bool status = _shape->getCapsuleGeometry(geometry);

	if (status)
	{
		radius = geometry.radius;
		halfHeight = geometry.halfHeight;
	}

	PxMat44 m(PxShapeExt::getGlobalPose(*_shape, *_actor));

	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
				m.column1.x, m.column1.y, m.column1.z, m.column1.w,
				m.column2.x, m.column2.y, m.column2.z, m.column2.w,
				m.column3.x, m.column3.y, m.column3.z, m.column3.w);


	M = glm::transpose(M);

	glm::vec3 position;
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec4 axis(halfHeight, 0, 0, 0); // axis for the capsule
	axis = M * axis; //rotating axis to correct orientation
	glm::vec4 colour = glm::vec4(0, 0, 0, 1);

	Gizmos::addSphere(position + axis.xyz(), radius, 10, 10, colour);
	Gizmos::addSphere(position - axis.xyz(), radius, 10, 10, colour);
	//fix the gizmo rotation
	glm::mat4 m2 = glm::rotate(M, 11 / 7.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	Gizmos::addCylinderFilled(position, radius, halfHeight, 10, colour, &m2);
}

void AddSphere(PxShape* _shape, PxRigidActor* _actor, glm::vec3 _startPos)
{
	PxSphereGeometry geometry;

	float radius = 1;

	bool status = _shape->getSphereGeometry(geometry);

	if (status)
	{
		radius = geometry.radius;
	}

	PxMat44 m(PxShapeExt::getGlobalPose(*_shape, *_actor));

	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	M = glm::transpose(M);

	glm::vec3 position;

	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec4 colour = glm::vec4(0, 0, 1, 1);

	Gizmos::addSphere(position, radius, (radius * 10), (radius * 10), colour, &M);
}

void PhysXApp::FireSphere(glm::mat4 _camTrans)
{
	float density = 20;

	PxSphereGeometry sphere(2);
	PxTransform sphereTrans(PxVec3(_camTrans[3].x, _camTrans[3].y, _camTrans[3].z));
	PxRigidDynamic* newSphere = PxCreateDynamic(*g_Physics, sphereTrans, sphere, *g_PhysicsMaterial, density);

	g_PhysicsScene->addActor(*newSphere);
	m_physxActors.push_back(newSphere);

	float muzzleSpeed = -50; //value suggest by tut

	glm::vec3 direction(_camTrans[2]);
	PxVec3 velocity = PxVec3(direction.x, direction.y, direction.z) * muzzleSpeed;

	newSphere->setLinearVelocity(velocity, true);
}

//TUTORIALS
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
	PxRigidDynamic* dynamicActorBox = PxCreateDynamic(*g_Physics, transform, box, *g_PhysicsMaterial, density);

	//add a sphere
	PxSphereGeometry sphere(2);
	PxTransform sphereTrans(PxVec3(0, 10, 0));
	PxRigidDynamic* dynamicActorSphere = PxCreateDynamic(*g_Physics, sphereTrans, sphere, *g_PhysicsMaterial, density);

	//add the actors to the physX scene
	g_PhysicsScene->addActor(*dynamicActorBox);
	g_PhysicsScene->addActor(*dynamicActorSphere);

	//add actors to local scene for rendering
	m_physxActors.push_back(dynamicActorBox);
	m_physxActors.push_back(dynamicActorSphere);
}

void PhysXApp::SetUpTutRagdoll()
{
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, -4, 0.0f), physx::PxQuat(PxHalfPi * 1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(), *g_PhysicsMaterial);
	
	//add the plane to the scene
	g_PhysicsScene->addActor(*plane);

	Ragdoll* ragdoll = new Ragdoll();

	PxArticulation* ragdollArticulation;
	ragdollArticulation = ragdoll->MakeRagdoll(g_Physics, ragdoll->m_ragdollData, PxTransform(PxVec3(0, 10, 0)), 0.1f, g_PhysicsMaterial);
	
	g_PhysicsScene->addArticulation(*ragdollArticulation);
}

void PhysXApp::SetupTutController()
{
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), physx::PxQuat(PxHalfPi * 1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(), *g_PhysicsMaterial);
	//add the plane to the scene
	g_PhysicsScene->addActor(*plane);

	//add a box for the trigger
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(0, 5, 0));
	PxRigidDynamic* dynamicActorBox = PxCreateDynamic(*g_Physics, transform, box, *g_PhysicsMaterial, 10);
	g_PhysicsScene->addActor(*dynamicActorBox);
	m_physxActors.push_back(dynamicActorBox);
	
	//PxCapsuleGeometry capsule(2, 4);
	//PxTransform capsuleTrans(PxVec3(0, 10, 0), PxQuat(PxPi / 2.0f, PxVec3(0, 0, 1.0f)));
	//m_capsule = PxCreateDynamic(*g_Physics, capsuleTrans, capsule, *g_PhysicsMaterial, 10);

	//m_capsule->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//g_PhysicsScene->addActor(*m_capsule);
	//m_physxActors.push_back(m_capsule);

	m_player = new PlayerController(g_PhysicsScene, g_PhysicsMaterial, window);

	//setting up triggers
	SetShapeAsTrigger(dynamicActorBox);
}

void PhysXApp::SetupTutFluid()
{
#pragma region Tub_Initialisation
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(), *g_PhysicsMaterial);
	const PxU32 numShapes = plane->getNbShapes();
	g_PhysicsScene->addActor(*plane);

	PxBoxGeometry side1(4.5f, 1, 0.5f);
	PxBoxGeometry side2(0.5f, 1, 4.5f);
	pose = PxTransform(PxVec3(0.0f, 0.5f, 4.0f));
	
	PxRigidStatic* box = PxCreateStatic(*g_Physics, pose, side1, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*box);
	m_physxActors.push_back(box);

	pose = PxTransform(PxVec3(0.0f, 0.5f, -4.0f));
	box = PxCreateStatic(*g_Physics, pose, side1, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*box);
	m_physxActors.push_back(box);

	pose = PxTransform(PxVec3(4.0f, 0.5f, 0.0f));
	box = PxCreateStatic(*g_Physics, pose, side2, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*box);
	m_physxActors.push_back(box);

	pose = PxTransform(PxVec3(-4.0f, 0.5f, 0.0f));
	box = PxCreateStatic(*g_Physics, pose, side2, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*box);
m_physxActors.push_back(box);
#pragma endregion

PxParticleFluid* pf;

PxU32 maxParticles = 2000;
bool perParticleResetOffset = false;
pf = g_Physics->createParticleFluid(maxParticles, perParticleResetOffset);

pf->setRestParticleDistance(0.3f);
pf->setDynamicFriction(0.1f);
pf->setStaticFriction(0.1f);
pf->setDamping(0.1f);
pf->setParticleMass(0.1f);
pf->setRestitution(0);
//pf->setParticleReadDataFlag(PxParticleReadDataFlag::eDENSITY_BUFFER, true);
pf->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
pf->setStiffness(100);

if (pf)
{
	g_PhysicsScene->addActor(*pf);

	m_particleEmitter = new ParticleFluidEmitter(maxParticles, PxVec3(0, 10, 0), pf, 0.1f);
	m_particleEmitter->setStartVelocityRange(-0.001f, -250.0f, -0.001f, 0.001f, 250.0f, 0.001f);
}
}

glm::vec3 PhysXApp::Vec3PhysXToGLM(PxVec3 &_vec)
{
	glm::vec3 vector;

	vector.x = _vec.x;
	vector.y = _vec.y;
	vector.z = _vec.z;

	return vector;
}

PxVec3 PhysXApp::Vec3GLMToPhysX(glm::vec3 &_vec)
{
	PxVec3 vector;

	vector.x = _vec.x;
	vector.y = _vec.y;
	vector.z = _vec.z;

	return vector;
}

//---------- PhysXApp ENDS ----------

//---------- MyControllerHitReport STARTS ----------

void myCollisionCallBack::onContact(const PxContactPairHeader& _pairHeader, const PxContactPair* _pairs, PxU32 _numPairs)
{
	for (PxU32 i = 0; i < _numPairs; i++)
	{
		const PxContactPair& cp = _pairs[i];

		//only interested in touches found and lost
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cout << "Collision Detection between: ";
			std::cout << _pairHeader.actors[0]->getName();
			std::cout << _pairHeader.actors[1]->getName() << endl;
		}
	}
}

void myCollisionCallBack::onTrigger(PxTriggerPair* _pairs, PxU32 _numPairs)
{
	for (PxU32 i = 0; i < _numPairs; i++)
	{
		PxTriggerPair* pair = _pairs + i;
		PxActor* triggerActor = pair->triggerActor;
		PxActor* otherActor = pair->otherActor;

		std::cout << otherActor->getName();
		std::cout << " Entered Trigger ";
		std::cout << triggerActor->getName() << endl;
	}
}
#include "TutCollisionDetection.h"
#include "Camera\MobileCamera.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "PhysicsObject.h"
#include "Plane.h"

void TutCollisionDetection::Startup()
{
	MobileCamera* camera = new MobileCamera(50.0f, 0.1f);
	camera->SetInputWindow(window);
	camera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	camera->LookAt(glm::vec3(0, 1, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_camera = camera;

	m_physicsScene = new PhysicsScene(glm::vec2(0, 0), 0.001f);

	m_myPlane = new Plane();

	m_mySphereA = new Sphere(glm::vec3(5, 0, 0), glm::vec3(-15, 0, 0), 1.0f, 1.0f, glm::vec4(1, 0, 1, 1));
	m_mySphereB = new Sphere(glm::vec3(-5, 0, 0), glm::vec3(15, 0, 0), 1.0f, 1.0f, glm::vec4(1, 0, 1, 1));

	m_physicsScene->AddActor(m_myPlane);
	m_physicsScene->AddActor(m_mySphereA);
	m_physicsScene->AddActor(m_mySphereB);
}

void TutCollisionDetection::Shutdown()
{

}

bool TutCollisionDetection::Update(double _dt)
{
	//m_camera->Update(_dt);

	m_physicsScene->Update();
	m_physicsScene->CheckForCollisions();

	return true;
}

void TutCollisionDetection::Render()
{
	Gizmos::addTransform(glm::mat4(1));

	m_physicsScene->RenderGizmos();
}
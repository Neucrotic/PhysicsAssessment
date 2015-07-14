#include "NewtonsThirdLaw.h"
#include "Camera\MobileCamera.h"
#include "PhysicsScene.h"
#include "Sphere.h"

void NewtonsThirdLaw::Startup()
{
	MobileCamera* camera = new MobileCamera(100.0f, 0.1f);
	camera->SetInputWindow(window);
	camera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	camera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, -10, 0), glm::vec3(0, 1, 0));
	m_camera = camera;

	physicsScene = new PhysicsScene(glm::vec2(0, -10), 0.001f);

	ballA = new Sphere(glm::vec3(-40, 0, 0), glm::vec3(0, 0, 0), 3.0f, 1, glm::vec4(1, 0, 0, 1));

	physicsScene->AddActor(ballA);
}

void NewtonsThirdLaw::Shutdown()
{

}

bool NewtonsThirdLaw::Update(double _dt)
{
	physicsScene->Update();

	return true;
}

void NewtonsThirdLaw::Render()
{
	physicsScene->RenderGizmos();
}
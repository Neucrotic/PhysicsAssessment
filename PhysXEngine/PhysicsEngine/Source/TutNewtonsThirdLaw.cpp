#include "TutNewtonsThirdLaw.h"
#include "Camera\MobileCamera.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "PhysicsObject.h"

void TutNewtonsThirdLaw::Startup()
{
	MobileCamera* camera = new MobileCamera(50.0f, 0.1f);
	camera->SetInputWindow(window);
	camera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	camera->LookAt(glm::vec3(0, 1, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_camera = camera;

	m_physicsScene = new PhysicsScene(glm::vec2(0, 0), 0.001f);

	m_ballA = new Sphere(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f, 3.0f, glm::vec4(1, 0, 0, 1));
	m_ballB = new Sphere(glm::vec3(6, 0, 0), glm::vec3(0, 0, 0), 1.0f, 3.0f, glm::vec4(0, 0, 1, 1));

	m_physicsScene->AddActor(m_ballA);
	m_physicsScene->AddActor(m_ballB);
}

void TutNewtonsThirdLaw::Shutdown()
{

}

bool TutNewtonsThirdLaw::Update(double _dt)
{
	//m_camera->Update(_dt);

	m_physicsScene->Update();

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_ballA->ApplyForceToActor(m_ballB, glm::vec3(5, 0, 0));
	}

	return true;
}

void TutNewtonsThirdLaw::Render()
{
	Gizmos::addTransform(glm::mat4(1));

	m_physicsScene->RenderGizmos();
}
#include "TutCollisionDetection.h"
#include "Camera\MobileCamera.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "PhysicsObject.h"
#include "Plane.h"
#include "Box.h"
#include "SpringJoint.h"

void TutCollisionDetection::Startup()
{
	MobileCamera* camera = new MobileCamera(50.0f, 0.1f);
	camera->SetInputWindow(window);
	camera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	camera->LookAt(glm::vec3(0, 1, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_camera = camera;

	m_physicsScene = new PhysicsScene(glm::vec2(0, 0), 0.001f);

	//m_mySphereA = new Sphere(glm::vec3(0, 5, 0), glm::vec3(0, 0, 0), 1.0f, 1.0f, glm::vec4(1, 0, 1, 1), false);
	//m_mySphereB = new Sphere(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), 1.0f, 1.0f, glm::vec4(1, 0, 1, 1), true);

	//m_myJoint = new SpringJoint(m_mySphereA, m_mySphereB, 10, 5);
	//m_physicsScene->AddActor(m_myJoint);
	//m_physicsScene->AddActor(m_mySphereA);
	//m_physicsScene->AddActor(m_mySphereB);

	//m_physicsScene->AddPlane(&m_myPlane, glm::vec3(0, 1, 0), 0);
	m_physicsScene->AddAABB(m_myBoxA, glm::vec3(10, -5, 0), glm::vec3(-200, 0, 0), 1.0f, glm::vec3(1, 1, 1));
	m_physicsScene->AddAABB(m_myBoxB, glm::vec3(-10, -5, 0), glm::vec3(200, 0, 0), 10.0f, glm::vec3(1, 1, 1));

	m_sphereActive = true;
}

void TutCollisionDetection::Shutdown()
{

}

bool TutCollisionDetection::Update(double _dt)
{
	m_camera->Update(_dt);

	m_physicsScene->Update();
	m_physicsScene->CheckForCollisions();

	if (m_keyCD <= 0)
	{
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (m_sphereActive)
			{
				FireSphere();
			}
			else
			{
				FireAABB();
			}
		}

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			m_sphereActive = !m_sphereActive;
		}

		m_keyCD = 0.15f;
	}

	m_keyCD -= _dt;

	return true;
}

void TutCollisionDetection::Render()
{
	Gizmos::addTransform(glm::mat4(1));

	m_physicsScene->RenderGizmos();
}

void TutCollisionDetection::FireSphere()
{
	Sphere* newSphere;

	float muzzleSpeed = -1000;
	glm::vec3 direction(m_camera->GetTransform()[2]);

	glm::vec3 vel = glm::vec3(direction.x * muzzleSpeed, direction.y * muzzleSpeed, direction.z * muzzleSpeed);
	glm::vec3 pos = m_camera->GetTransform()[3].xyz;

	m_physicsScene->AddSphere(newSphere, pos, vel, 5.0f, 2.0f);
}

void TutCollisionDetection::FireAABB()
{
	Box* newBox;

	float muzzleSpeed = -1000;
	glm::vec3 direction(m_camera->GetTransform()[2]);

	glm::vec3 vel = glm::vec3(direction.x * muzzleSpeed, direction.y * muzzleSpeed, direction.z * muzzleSpeed);
	glm::vec3 pos = m_camera->GetTransform()[3].xyz;

	m_physicsScene->AddAABB(newBox, pos, vel, 5.0f, glm::vec3(2.0f, 2.0f, 2.0f));
}
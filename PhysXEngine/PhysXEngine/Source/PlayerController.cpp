#include "PlayerController.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

PlayerController::PlayerController(PxScene* _scene, PxMaterial* _material, GLFWwindow* _window)
{
	m_window = _window; //saving the window

	//setting up the controllers
	m_myHitReport = new ControllerHitReport();
	PxControllerManager* characterManager = PxCreateControllerManager(*_scene);
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = _material;
	desc.reportCallback = m_myHitReport; //this connects it to collsion detection routine
	desc.density = 10;

	m_controller = characterManager->createController(desc);
	m_controller->setPosition(PxExtendedVec3(0, 5, 0));

	//initialise character related member variables
	m_characterYVelocity = 0;
	m_characterRotation = 0;
	m_playerGravity = -5.0f;
	
	m_myHitReport->ClearPlayerContactNormal();
	//_scene->addActor(*m_controller->getActor());
}

void PlayerController::MovePlayerController(double _dt)
{
	const PxVec3 up(0, 1, 0);
	bool onGround;
	float movementSpeed = 10.0f;
	float rotationSpeed = 1.0f;
	float minDistance = 0.001f;

	if (m_myHitReport->GetPlayerContactNormal().y > 0.3f)
	{
		m_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		m_characterYVelocity += -1 * _dt;
		onGround = false;
	}

	m_myHitReport->ClearPlayerContactNormal();
	PxVec3 velocity(0, m_characterYVelocity, 0);

	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_characterRotation += glm::radians(5.f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_characterRotation -= glm::radians(5.f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		velocity.z -= movementSpeed * _dt;
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		velocity.z += movementSpeed * _dt;
	}

	PxControllerFilters filter;

	PxQuat rotation(m_characterRotation, PxVec3(0, 1, 0));
	//velocity = PxVec3(0, m_characterYVelocity, 0);
	m_controller->move(rotation.rotate(velocity), minDistance, _dt, filter);
}

glm::vec3 PlayerController::GetPosition()
{
	glm::vec3 vec;

	vec.x = m_controller->getPosition().x;
	vec.y = m_controller->getPosition().y;
	vec.z = m_controller->getPosition().z;

	return vec;
}

glm::mat4x4 PlayerController::GetTransform()
{
	return glm::translate(GetPosition()) * glm::rotate(m_characterRotation, glm::vec3(0, 1, 0));
}

//---------- PhysXApp ENDS ----------

//---------- MyControllerHitReport STARTS ----------

void ControllerHitReport::onShapeHit(const PxControllerShapeHit &_hit)
{
	PxRigidActor* actor = _hit.shape->getActor();

	m_playerContactNormal = _hit.worldNormal;

	PxRigidDynamic* myActor = actor->is<PxRigidDynamic>();
	if (myActor != NULL)
	{

	}
}
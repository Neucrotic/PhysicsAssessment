#pragma once
#include <PxPhysicsAPI.h>
#include <pvd\PxVisualDebugger.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

using namespace physx;

class ControllerHitReport;

class PlayerController
{
public:

	PlayerController(PxScene* _scene, PxMaterial* _material, GLFWwindow* _window);

	void MovePlayerController(double _dt);

	glm::vec3 GetPosition();
	glm::mat4x4 GetTransform();

private:

	PxController* m_controller;
	ControllerHitReport* m_myHitReport;
	float m_characterYVelocity;
	float m_characterRotation;
	float m_playerGravity;

	//window used for input
	GLFWwindow* m_window;

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
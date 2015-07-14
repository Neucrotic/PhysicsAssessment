#pragma once
#include "Giz_core\Gizmos.h"
#include "Giz_core\gl_core_4_4.h"
#include "Camera\Camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

typedef unsigned int uint;

class App
{
public:

	int Run();

	virtual void Startup() = 0;
	virtual void Shutdown() = 0;

	virtual bool Update(double _dt) = 0;
	virtual void Render() = 0;

protected:

	GLFWwindow* window;
	Camera* m_camera;
	double deltaTime;

	bool Init();
	bool Deinit();
};
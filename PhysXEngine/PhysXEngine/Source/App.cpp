#include "App.h"

int App::Run()
{
	if (!Init())
	{
		return -1;
	}

	Startup();

	double currentTime = glfwGetTime();
	double lastFrameTime;
	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		lastFrameTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Update(deltaTime);

		Gizmos::clear();
		Render();
		Gizmos::draw(m_camera->GetProjectionView());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Deinit();

	return 0;
}

bool App::Init()
{
	if (glfwInit() == false)
	{
		return false;
	}

	window = glfwCreateWindow(720, 720, "PhysX", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();

		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glEnable(GL_DEPTH_TEST); //depth buffer now enabled

	Gizmos::create();

	return true;
}

bool App::Deinit()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#include "Application.h"


Application::Application()
{

}
Application::~Application()
{

}



bool Application::startup()
{
	if (glfwInit() == 0)
	{
		//window not opened
		return false;
	}

	//sets windows values
	this->m_window = glfwCreateWindow(1280, 720, "Draughts", nullptr, nullptr);

	if (this->m_window == nullptr)
	{
		//window failed to initalise
		return false;
	}

	//make above window current GLFW window
	glfwMakeContextCurrent(this->m_window);

	//loads opengl functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		//opengl load failed
		glfwDestroyWindow(this->m_window);
		glfwTerminate();
		return false;
	}

	int majorVersion = ogl_GetMajorVersion();
	int minorVersion = ogl_GetMinorVersion();

	printf("Successfully loaded OpenGL version %d.%d\n", majorVersion, minorVersion);


	return true;
}

void Application::shutdown()
{
	glfwDestroyWindow(this->m_window);
	glfwTerminate();
}


bool Application::update()
{
	if ((bool)(glfwWindowShouldClose(this->m_window)) == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Application::draw(){	}
#pragma once

#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <cstdio>

class GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

protected:
	GLFWwindow* m_window;

};

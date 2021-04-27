#include "context.h"
#include <iostream>

Context::Context(const char* windowName, float width, float height)
{
	init(windowName, width, height);
}

GLFWwindow* Context::getWindow() const
{
	return m_pWindow;
}

void Context::init(const char* windowName, float width, float height) noexcept
{
	initGlfw();
	if (!initWindowAndContext(windowName, width, height)) {
		glfwTerminate();
		return;
	}
}

void Context::initGlfw() noexcept
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}
}

bool Context::initWindowAndContext(const char* windowName, float width, float height) noexcept
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow = glfwCreateWindow(width, height, windowName, NULL, NULL);
	if (!m_pWindow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_pWindow);
	glfwSetFramebufferSizeCallback(m_pWindow, frameBufferWindowCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

void Context::frameBufferWindowCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#include "context.h"
#include <iostream>

bool Context::Keys[1024];

Context::Context(const char* windowName, float width, float height)
{
	std::cout << "i got created" << std::endl;
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
	glfwWindowHint(GLFW_RESIZABLE, false);

	m_pWindow = glfwCreateWindow(width, height, windowName, NULL, NULL);
	if (!m_pWindow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_pWindow);
	glfwSetKeyCallback(m_pWindow, keyCallback);
	glfwSetFramebufferSizeCallback(m_pWindow, frameBufferWindowCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Context::frameBufferWindowCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Context::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			Keys[key] = true;
		}
		else if(action == GLFW_RELEASE) {
			Keys[key] = false;
		}
	}
}

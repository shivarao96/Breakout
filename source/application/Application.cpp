#include "Application.h"
#include <iostream>

Application::Application(const char* windowName, float width, float height)
	:m_context(windowName, width, height)
	,m_screenWidth(width)
	,m_screenHeight(height)
{}

Application::~Application()
{
	glfwTerminate();
}

void Application::runApp()
{
	while (!glfwWindowShouldClose(m_context.getWindow())) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_context.getWindow());
		glfwPollEvents();
	}
}
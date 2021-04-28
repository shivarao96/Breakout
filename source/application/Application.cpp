#include "application.h"
#include <iostream>

Application::Application(const char* windowName, float width, float height)
	:m_context(windowName, width, height)
	,m_screenWidth(width)
	,m_screenHeight(height)
	,m_gameMechanic(width, height, m_context)
{
	m_gameMechanic.init();
}

Application::~Application()
{
	glfwTerminate();
}

void Application::runApp()
{
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(m_context.getWindow())) {
		
		// delta time calucation here
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// input here
		m_gameMechanic.processInput(deltaTime);
		
		// update here 
		m_gameMechanic.update(deltaTime);

		// render here
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// render here
		glfwSwapBuffers(m_context.getWindow());
	}
	m_gameMechanic.clear();
}
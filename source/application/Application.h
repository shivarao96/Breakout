#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include "../context/context.h"

class Application
{
public:
	Application(const char* windowName, float width, float height);
	~Application();
	void runApp();
private:
	float m_screenWidth;
	float m_screenHeight;
	
	context m_context;
};


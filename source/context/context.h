#pragma once
#include <glad/glad.h>
#include <glfw3.h>

class Context
{
public:
	Context(const char* windowName, float width, float height);
	GLFWwindow* getWindow() const;
	static bool Keys[1024];
private:
	void init(const char* windowName, float width, float height) noexcept;
	void initGlfw() noexcept;
	bool initWindowAndContext(const char* windowName, float width, float height) noexcept;
	static void frameBufferWindowCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	GLFWwindow* m_pWindow;
};


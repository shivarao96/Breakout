#pragma once
#include <glad/glad.h>
#include <glfw3.h>

class context
{
public:
	context(const char* windowName, float width, float height);
	GLFWwindow* getWindow() const;
private:
	void init(const char* windowName, float width, float height) noexcept;
	void initGlfw() noexcept;
	bool initWindowAndContext(const char* windowName, float width, float height) noexcept;
	static void frameBufferWindowCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* m_pWindow;
};


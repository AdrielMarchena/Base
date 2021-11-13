#pragma once

struct GLFWwindow;
namespace Base
{
class GLContext
{
private:
	bool m_Initialized = false;
	GLFWwindow* m_Window;
public:
	GLContext(GLFWwindow* window);
	~GLContext();

	void Init();
	void Shutdown();
	void SwapBuffer();
};
}


#include "GLContext.h"
#include "utils/base_assert.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
namespace Base
{
	GLContext::GLContext(GLFWwindow* window)
		:m_Window(window)
	{
		BASE_CORE_ASSERT(window, "Window is null");
	}
	GLContext::~GLContext()
	{
		Shutdown();
	}
	void GLContext::Init()
	{
		if (!m_Initialized)
		{
			glfwMakeContextCurrent(m_Window);
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			BASE_CORE_ASSERT(status, "GLAD Initialized!");
			BASE_INFO("OpenGL Info:");
			BASE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
			BASE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
			BASE_INFO("  Version: {0}", glGetString(GL_VERSION));
			BASE_INFO("  Shading Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
			int count = 0;
			glGetIntegerv(GL_EXTENSIONS, &count);
			BASE_INFO("  Extensions:");
			for (int i = 0; i < count; i++)
				BASE_INFO("  {0}: ", glGetStringi(GL_EXTENSIONS, i));
			
			m_Initialized = true;
		}

	}
	void GLContext::Shutdown()
	{
		if (m_Initialized)
		{
		}
	}
	void GLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_Window);
	}
}
#include "Window.hpp"

namespace cmgl
{

namespace priv
{
	bool glfw_initialized = false;

	void glfw_error(int error, const char* description)
	{
		fprintf(stderr, "GLFW error : %s\n", description);
	}

	void glfw_init()
	{
		if (!glfw_initialized)
		{
			glfwSetErrorCallback(glfw_error);
			
			glfwInit();

			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			glfw_initialized = true;
		}
	}

	// TODO : glfwTerminate();

	bool glew_initialized = false;

	void glew_init()
	{
		if (!glew_initialized)
		{
			glewExperimental = true; // Needed in core profile
			if (glewInit() != GLEW_OK)
			{
				fprintf(stderr, "GLEW error : Failed to initialize GLEW\n");
			}

			glew_initialized = true;
		}
	}

} // namespace priv

Window::Window()
	: mWindow(nullptr)
{
	priv::glfw_init();
}

Window::Window(unsigned int width, unsigned int height, const std::string& title)
	: mWindow(nullptr)
{
	priv::glfw_init();
	create(width, height, title);
}

Window::~Window()
{
	if (mWindow != nullptr)
	{
		glfwDestroyWindow(mWindow);
	}
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(mWindow);
	priv::glew_init();
}

bool Window::isContextCurrent() const
{
	return mWindow == glfwGetCurrentContext();
}

bool Window::create(unsigned int width, unsigned int height, const std::string& title)
{
	if (mWindow != nullptr)
	{
		glfwDestroyWindow(mWindow);
	}
	mWindow = glfwCreateWindow((int)width, (int)height, title.c_str(), nullptr, nullptr);
	if (mWindow == nullptr)
	{
		return false;
	}
	else
	{
		glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetInputMode(mWindow, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
		makeContextCurrent();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		return true;
	}
}

void Window::close()
{
	glfwSetWindowShouldClose(mWindow, GL_TRUE);
}

bool Window::isOpen() const
{
	return mWindow != nullptr && glfwWindowShouldClose(mWindow) == GL_FALSE;
}

glm::uvec2 Window::getSize() const
{
	int w, h;
	glfwGetWindowSize(mWindow, &w, &h);
	return glm::uvec2((unsigned int)w, (unsigned int)h);
}

glm::uvec2 Window::getFramebufferSize() const
{
	int w, h;
	glfwGetFramebufferSize(mWindow, &w, &h);
	return glm::uvec2((unsigned int)w, (unsigned int)h);
}

void Window::clear(const Color& color)
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display()
{
	glfwSwapBuffers(mWindow);
}

bool Window::hasFocus() const
{
	return glfwGetWindowAttrib(mWindow, GLFW_FOCUSED) == GLFW_TRUE;
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::waitEvents()
{
	glfwWaitEvents();
}

bool Window::isKeyPressed(int key) const
{
	return glfwGetKey(mWindow, key) == GLFW_PRESS;
}

bool Window::isMouseButtonPressed(int button) const
{
	return glfwGetMouseButton(mWindow, button) == GLFW_PRESS;
}

glm::vec2 Window::getCursorPos() const
{
	double x, y;
	glfwGetCursorPos(mWindow, &x, &y);
	return glm::vec2((float)x, (float)y);
}

void Window::setCursorPos(const glm::vec2& cursorPosition)
{
	glfwSetCursorPos(mWindow, (double)cursorPosition.x, (double)cursorPosition.y);
}

void Window::setCursorPos(float x, float y)
{
	glfwSetCursorPos(mWindow, (double)x, (double)y);
}

void Window::setCursorMode(int mode)
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, mode);
}

void Window::setClipboardText(const std::string& text)
{
	glfwSetClipboardString(mWindow, text.c_str());
}

std::string Window::getClipboardText() const
{
	return std::string(glfwGetClipboardString(mWindow));
}

#ifdef _WIN32
HWND Window::getWindowHandle()
{
	return glfwGetWin32Window(mWindow);
}
#endif

} // namespace cmgl
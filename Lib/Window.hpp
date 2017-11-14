#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Color.hpp"

#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

namespace cmgl
{

class Window
{
	public:
		Window();
		Window(unsigned int width, unsigned int height, const std::string& title);
		~Window();

		void makeContextCurrent();
		bool isContextCurrent() const;

		bool create(unsigned int width, unsigned int height, const std::string& title);
		void close();
		bool isOpen() const;

		glm::uvec2 getSize() const;
		glm::uvec2 getFramebufferSize() const;

		void clear(const Color& color = Color::Black);
		void display();

		bool hasFocus() const;

		void pollEvents();
		void waitEvents();

		bool isKeyPressed(int key) const;
		bool isMouseButtonPressed(int button) const;

		glm::vec2 getCursorPos() const;
		void setCursorPos(const glm::vec2& cursorPosition);
		void setCursorPos(float x, float y);
		void setCursorMode(int mode); // GLFW_NORMAL, GLFW_HIDDEN, GLFW_DISABLED

		void setClipboardText(const std::string& text);
		std::string getClipboardText() const;

		#ifdef _WIN32
		HWND getWindowHandle();
		#endif

	private:
		GLFWwindow* mWindow;

};

} // namespace cmgl
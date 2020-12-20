#pragma once

#include <GLFW/glfw3.h>

namespace avt {
	class Vector2;

	class App {
	public:
		App() {}
		virtual ~App () {}

		virtual void initCallback(GLFWwindow* win) {}
		virtual void displayCallback(GLFWwindow* win, double dt) {}
		virtual void updateCallback(GLFWwindow* win, double dt) {}
		virtual void updateMousePicker(double xcursor, double ycursor) {}
		virtual void pollEventsCallback(GLFWwindow* win, const Vector2& lastCursor, const Vector2& newCursor, double dt) {}
		virtual void windowResizeCallback(GLFWwindow* win, int w, int h) {}
		virtual void windowCloseCallback(GLFWwindow* win) {}
		virtual void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {}
		virtual void mouseButtonCallback(GLFWwindow* win, int button, int actions, int mods) {}

	};
}
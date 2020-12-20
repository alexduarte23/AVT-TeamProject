#pragma once

#include <iostream>

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace avt {
	class ErrorManager {
	public:
		bool _updated;

		ErrorManager() : _updated(false) {}

		ErrorManager(bool updated) : _updated(updated){}

		static const std::string errorSource(GLenum source);
		

		static const std::string errorType(GLenum type);
		

		static const std::string errorSeverity(GLenum severity);
		

		static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam);
		

		void setupErrorCallback();

		// OUTDATED VERSION

		static const std::string errorString(GLenum error);

		static bool isOpenGLError();

		static void checkOpenGLError(std::string error);

	};
}

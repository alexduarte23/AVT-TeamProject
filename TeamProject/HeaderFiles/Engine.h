#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "App.h"

#include "Renderer.h"
//#include "Shader.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "avt_math.h"
#include "ErrorManager.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

#include "Scene.h"
#include "SceneNode.h"

#include "Manager.h"

#include "Mesh.h"
#include "Cube.h"

#include "ParticleEmitter.h"


#define ERROR_CALLBACK

namespace avt {

	class Engine {
	private:
		GLFWwindow* _win = nullptr;

		avt::ErrorManager _errorManager;

		App* _app = nullptr;

		int _glMajor = 0;
		int _glMinor = 0;

		int _winX = 640;
		int _winY = 640;
		const char* _winTitle = "Undefined";
		int _fullscreen = 0;
		int _vsync = 1;

		bool _defaultApp = true;

		void setupGLFW();
		void setupWindow();
		void setupGLEW();
		void setupOpenGL();
		void checkOpenGLInfo();


	public:
		Engine() : _app(new App()) {}

		virtual ~Engine() {
			if (_defaultApp) delete _app;
		}

		App* app() {
			return _app;
		}

		void setApp(App* app) {
			_defaultApp = false;
			delete _app;
			_app = app;
		}

		void setOpenGL(int gl_major, int gl_minor) {
			_glMajor = gl_major;
			_glMinor = gl_minor;
		}

		void setWindow(int winx, int winy, const char* title, int is_fullscreen, int is_vsync) {
			_winX = winx;
			_winY = winy;
			_winTitle = title;
			_fullscreen = is_fullscreen;
			_vsync = is_vsync;
		}

		void init();
		void run();
		void shutdown();


		static void window_close_callback(GLFWwindow* win);
		static void window_size_callback(GLFWwindow* win, int winx, int winy);
		static void glfw_error_callback(int error, const char* description);
		static void window_key_callback(GLFWwindow* win, int key, int scanncode, int action, int mods);
	};

}
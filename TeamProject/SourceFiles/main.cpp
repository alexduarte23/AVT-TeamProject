#include <iostream>

#include "../HeaderFiles/Engine.h"

#include <vector>


class MyApp : public avt::App {
private:
	avt::Shader _shader, _shaderP;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene;

	avt::ParticleEmitter *_emitter;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;

	std::string _activeCam = "per";
	
	const GLuint UBO_BP = 0;
	const GLuint VERTICES = 0;
	const GLuint COLORS = 1;

	void createScene() {

		_scene.getRoot()->setShader(&_shader);

		auto cubeM = _meshes.add("cube", new avt::Cube());
		cubeM->colorAll({ 1.f,1.f,1.f,1.f });
		cubeM->setup();

		_ub.create(2 * 16 * sizeof(GLfloat), 0); // change
		_ub.unbind();

		auto plane = _scene.createNode(cubeM);
		plane->translate({ 0,20.f,0 });

		_emitter = new avt::ParticleEmitter();
		_emitter->setShader(&_shaderP);
		_scene.addNode(_emitter); // scene deletes nodes when destroyed

#ifndef ERROR_CALLBACK
		avt::ErrorManager::checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
	}
		

	void processKeyInput(GLFWwindow* win, double dt) {
		avt::Vector3 move;
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
			move.setZ(move.z() + 1);
		}
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
			move.setZ(move.z() - 1);
		}
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
			move.setX(move.x() - 1);
		}
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
			move.setX(move.x() + 1);
		}
		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {
			move.setY(move.y() + 1);
		}
		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			move.setY(move.y() - 1);
		}

		_cams.get("ort")->processMove(move, dt);
		_cams.get("per")->processMove(move, dt);
	}

	void processMouseMovement(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double  dt) {
		auto offset = newCursor - lastCursor;
		if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) { // free move
			_cams.get("ort")->processMouse(offset, dt, true);
			_cams.get("per")->processMouse(offset, dt, true);

		} else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { // drag move
			_cams.get("ort")->processMouse(offset, dt);
			_cams.get("per")->processMouse(offset, dt);
		}

	}

	void createShader() {
		_shader.addShader(GL_VERTEX_SHADER, "./Resources/vertexshader3d.shader");
		_shader.addShader(GL_FRAGMENT_SHADER, "./Resources/fragmentshader3d.shader");
		_shader.addAttribute("in_Position", VERTICES);
		_shader.addAttribute("in_Color", COLORS);
		_shader.addUniform("ModelMatrix");
		_shader.addUbo("SharedMatrices", UBO_BP);
		_shader.create();

		_shaderP.addShader(GL_VERTEX_SHADER, "./Resources/particles-vs.glsl");
		_shaderP.addShader(GL_FRAGMENT_SHADER, "./Resources/particles-fs.glsl");
		_shaderP.addAttribute("in_vertex", 0);
		_shaderP.addAttribute("in_texCoord", 1);
		_shaderP.addAttribute("in_pos", 2);
		_shaderP.addAttribute("in_color", 3);
		_shaderP.addAttribute("in_size", 4);
		_shaderP.addAttribute("in_rot", 5);
		_shaderP.addUniform("ModelMatrix");
		_shaderP.addUbo("SharedMatrices", UBO_BP);
		_shaderP.create();
	}

	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;
		_cams.add("per", new avt::PerspectiveCamera(45.f, aspect, 1.f, 100.0f, avt::Vector3(0, 8.f, 15.f)));
		_cams.add("ort", new avt::OrthographicCamera(-6.0f, 6.0f, -6.0f / aspect, 6.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 15.f)));

		_cams.get("ort")->setSpeed(12.f);
		_cams.get("per")->setSpeed(12.f);
		_cams.get("per")->lookAt(avt::Vector3());
	}

public:

	MyApp() : avt::App() {}

	~MyApp() {}

	void initCallback(GLFWwindow* win) override {
		createShader();
		createCams(win);
		createScene();
	}

	void pollEventsCallback(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double dt) override {
		processKeyInput(win, dt);
		processMouseMovement(win, lastCursor, newCursor, dt);
	}

	void updateCallback(GLFWwindow* win, double dt) override {
		avt::Mat4 rotMat;

		_emitter->update(dt);
		
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		_renderer.clear();
		//_renderer.draw(_scene, _ub, _shader, _cams.get(_activeCam));
		_scene.draw(_ub, _cams.get(_activeCam));
	}

	void windowResizeCallback(GLFWwindow* win, int w, int h) override {
		glViewport(0, 0, w, h);
		if (_cams.size() == 2) {
			_cams.get("ort")->resize(w, h);
			_cams.get("per")->resize(w, h);
		}
	}

	void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) override {
		if (action != GLFW_PRESS) return;

		switch (key) {
		case GLFW_KEY_P:
			_activeCam = _activeCam == "ort" ? "per" : "ort";
			break;
		case GLFW_KEY_ESCAPE:
			if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case GLFW_KEY_0:
			if (_cams.size() == 2) {
				_cams.get("ort")->setPosition(avt::Vector3(0, 0, 15.f));
				_cams.get("per")->setPosition(avt::Vector3(0, 0, 15.f));
				_cams.get("ort")->lookAt(avt::Vector3());
				_cams.get("per")->lookAt(avt::Vector3());
			}
			break;
		case GLFW_KEY_R:
			_emitter->toggle();
			break;
		}

	}

};

int main(int argc, char* argv[]) {
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;

	avt::App *app = new MyApp();
	avt::Engine engine;
	engine.setApp(app);
	engine.setOpenGL(gl_major, gl_minor);
	engine.setWindow(640, 480, "Penrose Museum", is_fullscreen, is_vsync);

	engine.init();
	engine.run();
	delete app;
	engine.shutdown();
	
	//exit(EXIT_SUCCESS);
	return 0;
}
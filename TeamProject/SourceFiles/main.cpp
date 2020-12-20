#include <iostream>

#include "../HeaderFiles/Engine.h"

#include <vector>

class MyNodeCallback : public avt::SceneNodeCallback {
public:
	void beforeDraw() override {
		//glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}

	void afterDraw() override {
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
};

class MyApp : public avt::App {
private:

	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene;
	MyNodeCallback nodeCallback;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;
	avt::Manager<avt::Shader> _shaders;
	avt::Bloom* bloom;

	avt::SceneNode* _ball_1 = nullptr, * _ball_2 = nullptr, * _ball_3 = nullptr;

	std::string _activeCam = "ort";
	
	const float _duration = 3, _duration2 = 6, _duration3 = 6;
	double _time = 0, _time2 = 0, _time3 = 0;
	bool _animating = false, _rotating = false, _moreblur = false, _lessblur = false;

	void createScene() {


		auto ball = _meshes.add("ball", new avt::Mesh("./Resources/bloomball.obj"));
		ball->setup();
		_ball_1 = _scene.createNode(ball);
		_ball_2 = _scene.createNode(ball);
		_ball_3 = _scene.createNode(ball);

		_ball_1->scale({ 2.0f, 2.0f, 2.0f });

		_ball_2->scale({ 2.0f, 2.0f, 2.0f });
		_ball_1->translate({ 0.0f, 0.0f, 3.0f });

		_ball_3->scale({ 2.0f, 2.0f, 2.0f });
		_ball_3->translate({ 0.0f, 0.0f, -3.0f });

		_ub.create(2 * 16 * sizeof(GLfloat), 0); // change
		_ub.unbind();

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

	void createBloom() {
		bloom = new avt::Bloom();
		bloom->create(WIDTH, HEIGHT);
	}

	void createShader() {
		avt::Shader* shader = new avt::Shader();
		shader->addShader(GL_VERTEX_SHADER, "./Resources/vertexshader3d.shader");
		shader->addShader(GL_FRAGMENT_SHADER, "./Resources/fragmentshader3d.shader");
		shader->addAttribute("inPosition", VERTICES);
		shader->addAttribute("inTexcoord", TEXTURES);
		shader->addAttribute("inNormal", NORMALS);
		shader->addUniform("ModelMatrix");
		shader->addUbo("SharedMatrices", UBO_BP);
		shader->create();
		_shaders.add("shader1", shader);
	}

	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;
		_cams.add("per", new avt::PerspectiveCamera(45.f, aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f)));
		_cams.add("ort", new avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 20.f)));

		_cams.get("ort")->setSpeed(12.f);
		_cams.get("per")->setSpeed(12.f);
	}

public:

	MyApp() : avt::App() {}

	~MyApp() {}

	void initCallback(GLFWwindow* win) override {
		createShader();
		createCams(win);
		createBloom();
		createScene();
		
	}

	void pollEventsCallback(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double dt) override {
		processKeyInput(win, dt);
		processMouseMovement(win, lastCursor, newCursor, dt);
	}

	void updateCallback(GLFWwindow* win, double dt) override {
		avt::Mat4 rotMat;

		if (_rotating) {
			_time2 += dt;
			if (_time2 > _duration2) {
				_time2 = 0;
				_rotating = false;
				_ball_1->setRotation(avt::Quaternion({ 1.f,1.f,0 }, 0));
				_ball_2->setRotation(avt::Quaternion({ 1.f,1.f,0 }, 0));
				_ball_3->setRotation(avt::Quaternion({ 1.f,1.f,0 }, 0));

			}
			float k = (float)_time2 / _duration2;
			_ball_1->setRotation(avt::Quaternion({ 0,1.f,0.f }, k * 2 * avt::PI));
			_ball_2->setRotation(avt::Quaternion({ 0,1.f,0.f }, k * 2 * avt::PI));
			_ball_3->setRotation(avt::Quaternion({ 0,1.f,0.f }, k * 2 * avt::PI));
		}
		if (_moreblur) {
			bloom->setBlurTex(1.0);
		}
		if (_lessblur) {
			bloom->setBlurTex(-1.0);
		}
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		_renderer.clear();

		bloom->bindHDR();
		_renderer.draw(_scene, _ub, *_shaders.get("shader1") , _cams.get(_activeCam));
		bloom->unbindHDR();
		
		bloom->bindPingBlur();
		bloom->renderHDR();
		bloom->unbindPingBlur();

		bloom->renderBlur();

		bloom->renderBloomFinal();

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
		case GLFW_KEY_C:
			_animating = !_animating;
			break;
		case GLFW_KEY_F:
			_rotating = !_rotating;
			break;
		case GLFW_KEY_M:
			_moreblur = !_moreblur;
			break;
		case GLFW_KEY_L:
			_lessblur = !_lessblur;
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
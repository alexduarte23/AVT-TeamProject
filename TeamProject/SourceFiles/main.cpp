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
	avt::Shader _shader, _shaderP;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene;
	MyNodeCallback nodeCallback;

	avt::ParticleEmitter *_emitter;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;

	avt::SceneNode *_cubeStruct=nullptr, *_frame=nullptr, *_panel=nullptr;
	avt::SceneNode* _cube1 = nullptr, * _cube2 = nullptr, * _cube3 = nullptr;
	avt::SceneNode* _cube4 = nullptr, * _cube5 = nullptr, * _cube6 = nullptr;
	avt::SceneNode* _cube7 = nullptr, * _cube8 = nullptr, * _cube9 = nullptr;

	std::string _activeCam = "per";
	
	const float _duration = 3, _duration2 = 6;
	double _time = 0, _time2 = 0;
	bool _animating = false, _rotating = false;

	const GLuint UBO_BP = 0;
	const GLuint VERTICES = 0;
	const GLuint COLORS = 1;

	void createScene() {

		_scene.getRoot()->setShader(&_shader);

		auto cubeM = _meshes.add("cube", new avt::Cube());
		cubeM->colorAll({ 1.f,1.f,1.f,1.f });
		auto frameM = _meshes.add("frame", new avt::Mesh("./Resources/frame.obj"));
		auto panelM = _meshes.add("panel", new avt::Mesh("./Resources/backpanel.obj"));
		frameM->colorAll(avt::Vector4(0.396f, 0.263f, 0.129f, 1.f));
		panelM->colorAll(avt::Vector4(0.1f, 0.1f, 0.1f, 1.f));

		cubeM->setup();
		frameM->setup();
		panelM->setup();

		_ub.create(2 * 16 * sizeof(GLfloat), 0); // change
		_ub.unbind();

		auto plane = _scene.createNode(cubeM);
		plane->translate({ 0,-1.f,0 });
		plane->scale({10.f,0.1f,10.f});

		_emitter = new avt::ParticleEmitter();
		_emitter->setShader(&_shaderP);
		_scene.addNode(_emitter); // scene deletes nodes when destroyed

		/*
		_frame = _scene.createNode(frameM);

		_panel = _frame->createNode(panelM);
		_panel->scale({ 1.9f, 1.9f, 1.9f });
		_panel->translate({ 0.0f, 0.0f, -0.3f });
		
		_cubeStruct = _frame->createNode();
		
		_cube9 = _cubeStruct->createNode(cubeM);
		_cube9->translate({ 9.0f, 6.0f, 9.f });

		_cube8 = _cubeStruct->createNode(cubeM);
		_cube8->translate({ 9.0f, 6.0f, 6.f });

		_cube1 = _cubeStruct->createNode(cubeM);
		_cube1->translate({ 0.0f, 0.0f, 0.f });
		_cube1->setCallback(&nodeCallback);

		_cube2 = _cubeStruct->createNode(cubeM);
		_cube2->translate({ 0.0f, 3.0f, 0.f });

		_cube3 = _cubeStruct->createNode(cubeM);
		_cube3->translate({ 0.0f, 6.0f, 0.f });

		_cube4 = _cubeStruct->createNode(cubeM);
		_cube4->translate({ 3.0f, 6.0f, 0.f });
		_cube4->setShader(&_shader2);

		_cube5 = _cubeStruct->createNode(cubeM);
		_cube5->translate({ 6.0f, 6.0f, 0.f });

		_cube6 = _cubeStruct->createNode(cubeM);
		_cube6->translate({ 9.0f, 6.0f, 0.f });

		_cube7 = _cubeStruct->createNode(cubeM);
		_cube7->translate({ 9.0f, 6.0f, 3.f });


		_cubeStruct->scale({ 0.5f, 0.5f, 0.5f });
		_cubeStruct->rotate(avt::Quaternion(avt::Vector3(0, 1.f, 0), avt::toRad(-55))
			* avt::Quaternion(avt::Vector3(0, 0, 1.f), avt::toRad(-45)));
		_cubeStruct->translate({ 0.1f, -1.4f, 2.15f });
		*/
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
		//_shaderP.addAttribute("in_Position", VERTICES);
		//_shaderP.addAttribute("in_Color", COLORS);
		_shaderP.addAttribute("in_vertex", 0);
		_shaderP.addAttribute("in_pos", 1);
		_shaderP.addAttribute("in_color", 2);
		_shaderP.addAttribute("in_size", 3);
		_shaderP.addUniform("ModelMatrix");
		_shaderP.addUbo("SharedMatrices", UBO_BP);
		_shaderP.create();
	}

	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;
		_cams.add("per", new avt::PerspectiveCamera(45.f, aspect, 1.f, 50.0f, avt::Vector3(0, 8.f, 15.f)));
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
		

		/*
		if (_animating) {
			_time += dt;
			if (_time > _duration) {
				_time -= _duration;

				_cube9->setTranslation({ 9.0f, 6.0f, 9.f });
				_cube8->setTranslation({ 9.0f, 6.0f, 6.f });
				_cube1->setTranslation({ 0.0f, 0.0f, 0.f });
				_cube2->setTranslation({ 0.0f, 3.0f, 0.f });
				_cube3->setTranslation({ 0.0f, 6.0f, 0.f });
				_cube4->setTranslation({ 3.0f, 6.0f, 0.f });
				_cube5->setTranslation({ 6.0f, 6.0f, 0.f });
				_cube6->setTranslation({ 9.0f, 6.0f, 0.f });
				_cube7->setTranslation({ 9.0f, 6.0f, 3.f });
			}
			_cube1->translate({ 0, (float)dt, 0 });
			_cube2->translate({ 0, (float)dt, 0 });
			_cube3->translate({ (float)dt, 0, 0 });
			_cube4->translate({ (float)dt, 0, 0 });
			_cube5->translate({ (float)dt, 0, 0 });
			_cube6->translate({ 0, 0, (float)dt });
			_cube7->translate({ 0, 0, (float)dt });
			_cube8->translate({ 0, 0, (float)dt });
			_cube9->translate({ .001f, (float)dt + .001f, 0 });
		}

		if (_rotating) {
			_time2 += dt;
			if (_time2 > _duration2) {
				_time2 = 0;
				_rotating = false;
				_frame->setRotation(avt::Quaternion({ 1.f,0,0 }, 0));
			}
			float k = (float)_time2 / _duration2;
			_frame->setRotation(avt::Quaternion({ 0,0,1.f }, k * 2 * avt::PI));
		}*/
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
		case GLFW_KEY_C:
			_animating = !_animating;
			break;
		case GLFW_KEY_F:
			_rotating = !_rotating;
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
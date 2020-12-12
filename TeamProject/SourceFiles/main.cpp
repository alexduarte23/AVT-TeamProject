#include <iostream>

#include "../HeaderFiles/Engine.h"
#include "../HeaderFiles/Globals.h"

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
	avt::Shader _shader;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene;
	MyNodeCallback nodeCallback;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;
	avt::Manager<avt::Light> _lights;

	avt::SceneNode* _tree = nullptr, *_lightStruct = nullptr , *_light= nullptr, *_floor = nullptr;
	std::string _activeCam = "ort";
	
	const float _duration = 3, _duration2 = 6;
	double _time = 0, _time2 = 0;
	bool _animating = false, _rotating = false;

	void createScene() {

		auto treeM = _meshes.add("tree", new avt::Mesh("./Resources/treeNormal.obj"));
		treeM->colorAll({0.2f, 0.6f, 0.2f});
		treeM->setup();

		auto floorM = _meshes.add("floor", new avt::Mesh("./Resources/cube_vtn_flat.obj"));
		floorM->setup();

		
		auto lightM = _meshes.add("cube", new avt::Mesh("./Resources/cube_vtn_flat.obj"));
		lightM->applyTransform(avt::Mat4::scale({ 0.25f, 0.25f, 0.25f }));
		lightM->setup();

		//CAMS
		_ub.create(2 * 16 * sizeof(GLfloat), 0); // change
		_ub.unbind();
		
		_tree = _scene.createNode(treeM);

		_lightStruct = _scene.createNode();

		_light = _lightStruct->createNode(lightM);
		_light->setTranslation(_lights.get("sun")->getPosition());
		
		_floor = _scene.createNode(floorM);
		_floor->scale({3.f, 1.0f, 3.f});
		_floor->translate({ 0.0f, -2.8f, 0.0f });
		


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
		_shader.addAttribute("inPosition", VERTICES);
		_shader.addAttribute("inTexcoord", TEXTURES);
		_shader.addAttribute("inNormal", NORMALS);
		_shader.addUniform("ModelMatrix");
		_shader.addUniform("LightPosition");
		_shader.addUniform("LightColor");
		_shader.addUbo("CameraMatrices", UBO_BP);
		_shader.create();
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

	void createLights() {
		_lights.add("sun", new avt::Light({ 4.f, 0.0f, 0.0f }, { 1.f, 1.f, 1.f }));
	}

public:

	MyApp() : avt::App() {}

	~MyApp() {}

	void initCallback(GLFWwindow* win) override {
		createShader();
		createCams(win);
		createLights();
		createScene();
	}

	void pollEventsCallback(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double dt) override {
		processKeyInput(win, dt);
		processMouseMovement(win, lastCursor, newCursor, dt);
	}

	void updateCallback(GLFWwindow* win, double dt) override {
		
		avt::Mat4 rotMat;
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
			/*
			_cube1->translate({ 0, (float)dt, 0 });
			_cube2->translate({ 0, (float)dt, 0 });
			_cube3->translate({ (float)dt, 0, 0 });
			_cube4->translate({ (float)dt, 0, 0 });
			_cube5->translate({ (float)dt, 0, 0 });
			_cube6->translate({ 0, 0, (float)dt });
			_cube7->translate({ 0, 0, (float)dt });
			_cube8->translate({ 0, 0, (float)dt });
			_cube9->translate({ .001f, (float)dt + .001f, 0 });
			
			_lights.get("sun")->setPosition({ 3.f, 3.f, (float)_time });
			_light->setTranslation(_lights.get("sun")->getPosition());
		}
		*/
		
		if (_rotating) {
			_time2 += dt;
			if (_time2 > _duration2) {
				_time2 = 0;
				_rotating = false;
				_lightStruct->setRotation(avt::Quaternion({ 1.f,0,0 }, 0));
			}
			float k = (float)_time2 / _duration2;
			_lightStruct->setRotation(avt::Quaternion({ 0,0,1.f }, k * 2 * avt::PI));
			_lights.get("sun")->setPosition(_light->pos().to3D());
		}
		
		
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		_renderer.clear();
		_renderer.draw(_scene, _ub, _shader, _cams.get(_activeCam), _lights.get("sun"));
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
#include <iostream>

#include "../HeaderFiles/Engine.h"

#include <vector>
#include <cassert>

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

	avt::SceneNode* _cubeStruct = nullptr;
	avt::SceneNode* _cube1 = nullptr, * _cube2 = nullptr, * _cube3 = nullptr;

	std::string _activeCam = "per";
	
	const float _duration = 3, _duration2 = 6;
	double _time = 0, _time2 = 0;
	bool _animating = false, _rotating = false, _selecting = false;

	//Stencil buffer mouse pcking
	unsigned int selected = -1;
	//

	const GLuint UBO_BP = 0;
	const GLuint VERTICES = 0;
	const GLuint COLORS = 1;

	void createScene() {

		auto cubeM1 = _meshes.add("cube1", new avt::Cube());
		auto cubeM2 = _meshes.add("cube2", new avt::Cube());
		auto cubeM3 = _meshes.add("cube3", new avt::Cube());

		// if object is selectable set Stencil Index

		cubeM1->colorAll(avt::Vector4(0.9f, 0.1f, 0.1f, 1.0f));
		cubeM2->colorAll(avt::Vector4(0.1f, 0.9f, 0.1f, 1.0f));
		cubeM3->colorAll(avt::Vector4(0.1f, 0.1f, 0.9f, 1.0f));

		cubeM1->setup();
		cubeM2->setup();
		cubeM3->setup();

		_ub.create(2 * 16 * sizeof(GLfloat), 0); // change
		_ub.unbind();

		_cubeStruct = _scene.createNode();
		
		_cube1 = _cubeStruct->createNode(cubeM1);

		_cube2 = _cubeStruct->createNode(cubeM2);
		_cube2->rotate(avt::Quaternion(avt::Vector3(0.0f, 1.f, 0.0f), avt::toRad(55)));
		_cube2->translate({ 4.0f,0.0f,0.0f });
		
		_cube3 = _cubeStruct->createNode(cubeM3);
		_cube3->rotate(avt::Quaternion(avt::Vector3(0, 1.f, 0), avt::toRad(-55)));
		_cube3->translate({ -4.0f,0.0f,0.0f});

		//Stencil buffer mouse pcking
		_cube1->setStencilIndex(2);
		_cube2->setStencilIndex(3);
		_cube3->setStencilIndex(1);
		//


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
		//Stencil buffer mouse picking
		else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			int x = static_cast<int>(newCursor.x());
			int y = 480 - static_cast<int>(newCursor.y());

			if (!_selecting) {
				//GLfloat color[4];
				//glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, color);
				//GLfloat depth;
				//glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
				GLuint index;
				glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
				std::cout << "cube number =" << index << std::endl;
				selected = index;

				_selecting = true;
			}
		}
		else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
			_selecting = false;
		}
		//

	}

	void createShader() {
		_shader.addShader(GL_VERTEX_SHADER, "./Resources/vertexshader3d.shader");
		_shader.addShader(GL_FRAGMENT_SHADER, "./Resources/fragmentshader3d.shader");
		_shader.addAttribute("in_Position", VERTICES);
		_shader.addAttribute("in_Color", COLORS);
		_shader.addUniform("Color");
		_shader.addUniform("ModelMatrix");
		
		_shader.addUbo("SharedMatrices", UBO_BP);
		_shader.create();
	}

	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;
		_cams.add("per", new avt::PerspectiveCamera(30.f, aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f)));
		_cams.add("ort", new avt::OrthographicCamera(-6.0f, 6.0f, -6.0f / aspect, 6.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 15.f)));

		_cams.get("ort")->setSpeed(12.f);
		_cams.get("per")->setSpeed(12.f);
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

		if (_animating) {
			_time += dt;
			if (_time > _duration) {
				_time -= _duration;

				_cube1->setTranslation({ 0.0f, 0.0f, 0.f });
				_cube2->setTranslation({ 0.0f, 3.0f, 0.f });
				_cube3->setTranslation({ 0.0f, 6.0f, 0.f });

			}
			_cube1->translate({ 0, (float)dt, 0 });
			_cube2->translate({ 0, (float)dt, 0 });
			_cube3->translate({ (float)dt, 0, 0 });

		}

		if (_rotating) {
			_time2 += dt;
			if (_time2 > _duration2) {
				_time2 = 0;
				_rotating = false;
				_cubeStruct->setRotation(avt::Quaternion({ 1.f,0,0 }, 0));
			}
			float k = (float)_time2 / _duration2;
			_cubeStruct->setRotation(avt::Quaternion({ 0,0,1.f }, k * 2 * avt::PI));
		}
	}

	void drawScene() {

		checkMousePicking();

		_renderer.draw(_scene, _ub, _shader, _cams.get(_activeCam));
	}

	void checkMousePicking()
	{
		for (auto childNode : _cubeStruct->children()) {
			if (childNode->getStencilIndex() == selected)
				childNode->selected(true);
			else
				childNode->selected(false);
		}
	}

	void displayCallback(GLFWwindow* win, double dt) override {

		_renderer.clear();
		drawScene();
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
	engine.setWindow("Penrose Museum", is_fullscreen, is_vsync);

	engine.init();
	engine.run();
	delete app;
	engine.shutdown();
	
	//exit(EXIT_SUCCESS);
	return 0;
}
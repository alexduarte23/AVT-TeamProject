#include <iostream>

#include "../HeaderFiles/Engine.h"
#include "../HeaderFiles/Globals.h"

#include <vector>

#include "../HeaderFiles/Shadow.h"
#include "../HeaderFiles/TerrainPlane.h"
#include "../HeaderFiles/Cloud.h"

class MyNodeCallback : public avt::SceneNodeCallback {
public:
	void beforeDraw() override {
		//glDisable(GL_CULL_FACE);
		//glDisable(GL_DEPTH_TEST);
	}

	void afterDraw() override {
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_TEST);
	}
};

class MyApp : public avt::App {
private:
	avt::Shader _shader;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene;
	MyNodeCallback nodeCallback;

	avt::Shadow _shadow;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;
	avt::Manager<avt::Light> _lights;


	avt::SceneNode* _tree = nullptr, *_lightStruct = nullptr , *_light= nullptr, *_floor = nullptr, *_cloud = nullptr;
	std::string _activeCam = "per";
	
	const float _duration = 3, _duration2 = 6;
	double _time = 0, _time2 = 0;
	bool _animating = false, _rotating = false, _selecting = false;

	//Stencil buffer mouse pcking
	unsigned int _selected = -1; //stencil index of the currently selected scene node 
	//

	void createScene() {

		auto treeM = _meshes.add("tree", new avt::Mesh("./Resources/treeNormal.obj"));
		treeM->colorAll({0.2f, 0.6f, 0.2f});
		treeM->setup();

		//auto floorM = _meshes.add("floor", new avt::Mesh("./Resources/cube_vtn_flat.obj"));
		auto floorM = _meshes.add("floor", new avt::TerrainPlane());
		floorM->setup();

		auto cloudM = _meshes.add("cloud", new avt::Cloud());
		cloudM->setup();

		
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
		_lightStruct->setRotation(avt::Quaternion({ 0,0,1.f }, avt::PI/4));

		_floor = _scene.createNode(floorM);
		//_floor->scale({6.f, 1.0f, 6.f});
		//_floor->translate({ 0.0f, -2.8f, 0.0f });
		_floor->translate({ -4.5f, -1.8f, -4.5f });

		//_cloud = _scene.createNode(cloudM);
		//_cloud->translate({ -2.5f, 4.f, -2.5f });

		setStencilIndex();
		

#ifndef ERROR_CALLBACK
		avt::ErrorManager::checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
	}

	//set Stencil Index of selectable scene nodes
	void setStencilIndex()
	{
		//example
		//scenenode->setStencilIndex(index);
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
				std::cout << "stencil index = " << index << std::endl;
				_selected = index;

				_selecting = true;
			}
		}
		else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
			_selecting = false;
		}
		//

	}

	void createShader() {
		_shader.addShader(GL_VERTEX_SHADER, "./Resources/vertexShadowShader.glsl");
		_shader.addShader(GL_FRAGMENT_SHADER, "./Resources/fragmentShadowShader.glsl");
		_shader.addAttribute("inPosition", VERTICES);
		_shader.addAttribute("inTexcoord", TEXTURES);
		_shader.addAttribute("inNormal", NORMALS);
		_shader.addAttribute("inColor", COLORS);
		_shader.addUniform("ModelMatrix");
		_shader.addUniform("lightSpaceMatrix");
		_shader.addUniform("shadowMap");
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

	void createShadows(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;

		_shadow = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 20.f)));
		_shadow.setPosition({ 6.0f, 0.0f, 0.0f });
		_shadow.lookAt({ 0.0f, 0.0f, 0.0f });
		_shadow.setup();
	}

	void createLights() {
		_lights.add("sun", new avt::Light({ 8.0f, 0.0f, 0.0f }, { 1.f, 1.f, 1.f }));
	}

public:

	MyApp() : avt::App() {}

	~MyApp() {}

	void initCallback(GLFWwindow* win) override {
		createCams(win);
		createShadows(win);
		createShader();
		createLights();
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
			}
		}
		
		if (_rotating) {
			_time2 += dt;
			if (_time2 > _duration2) {
				_time2 = 0;
				_rotating = false;
				_lightStruct->setRotation(avt::Quaternion({ 1.f,0,0 }, avt::PI/4));
			}
			float k = (float)_time2 / _duration2;
			_lightStruct->setRotation(avt::Quaternion({ 0,1.f,0.f }, k * 2 * avt::PI));
			_lightStruct->rotateZ(avt::PI/4);
		}

		_lights.get("sun")->setPosition(_light->pos().to3D());
		_shadow.setPosition(_light->pos().to3D());
		_shadow.lookAt({ 0.0f, 0.0f, 0.0f });
		_shader.bind();
		glUniformMatrix4fv(_shader.getUniform("lightSpaceMatrix"), 1, GL_FALSE, (_shadow._lightView.projMatrix() * _shadow._lightView.viewMatrix()).GLdata()); //TODO private stuff here
		_shader.unbind();
		
		
	}

	void checkMousePicking()
	{
		/** /
		for (auto childNode : _cubeStruct->children()) {
			if (childNode->getStencilIndex() == selected)
				childNode->selected(true);
			else
				childNode->selected(false);
		}
		/**/
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);
		_renderer.clear();

		checkMousePicking();

		_shadow.renderToDepthMap(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);
		glBindTexture(GL_TEXTURE_2D, _shadow.depthMap());
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
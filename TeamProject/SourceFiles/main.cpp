#include <iostream>

#include "../HeaderFiles/Engine.h"
#include "../HeaderFiles/Globals.h"

#include <vector>

#include "../HeaderFiles/Shadow.h"
#include "../HeaderFiles/TerrainPlane.h"


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
	avt::Shader _shader, _shaderP;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene;
	MyNodeCallback nodeCallback;

	avt::ParticleEmitter* _emitter = nullptr;

	avt::Shadow _shadow, _shadow2, _shadow3, _shadow4;
	avt::Bloom* _bloom = nullptr;
	avt::PointLight campfire;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;
	avt::Manager<avt::Light> _lights;


	avt::SceneNode* _tree = nullptr, *_tree2 = nullptr, * _tree3 = nullptr, *_lightStruct = nullptr , *_light= nullptr, *_floor = nullptr, *_cloud = nullptr;
	avt::CloudSystem* _cloudSystem = nullptr;
	std::string _activeCam = "per";
	
	const float _duration = 3, _duration2 = 6;
	double _time = 0, _time2 = 0;
	bool _animating = false, _rotating = false, _selecting = false, _morebloom = false, _lessbloom = false, _turnOffOnBloom = false;

	unsigned int _selected = -1; //stencil index of the currently selected scene node - mouse picking

	void createScene() {

		avt::StencilPicker::enable();

		auto treeM = _meshes.add("tree", new avt::Mesh("./Resources/Objects/treeNormal.obj"));
		treeM->colorAll({0.2f, 0.6f, 0.2f});
		treeM->setup();

		//auto floorM = _meshes.add("floor", new avt::Mesh("./Resources/cube_vtn_flat.obj"));
		auto floorM = _meshes.add("floor", new avt::TerrainPlane());
		floorM->setup();

		//auto cloudM = _meshes.add("cloud", new avt::Cloud());
		//cloudM->setup();

		
		auto lightM = _meshes.add("moon", new avt::Mesh("./Resources/Objects/sun_moon.obj"));
		lightM->applyTransform(avt::Mat4::scale({ 0.5f, 0.5f, 0.5f }));
		lightM->colorAll({ 1.f, 1.f, 1.f });
		lightM->setup();

		auto colorCubeM = _meshes.add("colorCube", new avt::Mesh("./Resources/Objects/colourscube.obj"));
		colorCubeM->setup();

		//CAMS
		_ub.create(2 * 16 * sizeof(GLfloat), 0); // change
		_ub.unbind();

		_scene.setShader(&_shader);


		_tree = _scene.createNode(treeM);
		//_tree->setStencilIndex(1);

		_tree2 = _scene.createNode(treeM);
		_tree2->translate({ 3.f, 0.f, 3.f });

		_tree3 = _scene.createNode(treeM);
		_tree3->translate({ -6.f, 0.f, -3.f });

		_lightStruct = _scene.createNode();

		_light = _lightStruct->createNode(lightM);
		//_light->setTranslation(_lights.get("sun")->getPosition());
		_light->setTranslation(campfire.getPosition());
		//_lightStruct->setRotation(avt::Quaternion({ 0,0,1.f }, avt::PI/10));

		_floor = _scene.createNode(floorM);
		_floor->translate({ -15.f, -2.f, -15.f });
		_floor->scale({3.f, 1.0f, 3.f});
		//_floor->translate({ 0.0f, -2.8f, 0.0f });

		//_cloud = _scene.createNode(cloudM);
		//_cloud->translate({ -2.5f, 4.f, -2.5f });

		auto colorCube = _scene.createNode(colorCubeM);
		colorCube->translate({ 0,0,5.f });
		//colorCube->rotateY(-avt::PI/2);
		colorCube->scale({ .3f,.3f,.3f });

		_cloudSystem = new avt::CloudSystem();
		_scene.addNode(_cloudSystem);
		_cloudSystem->translate({ 3.f,8.f,-3.f });
		_cloudSystem->scale({ .5f,.5f,.5f });

		_emitter = new avt::FireEmitter();
		avt::StencilPicker::addTarget(_emitter, "fire");
		_emitter->setShader(&_shaderP);
		//_emitter->scale({ .2f, .2f, .2f });
		_emitter->translate(campfire.getPosition());
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
		_shader.addShader(GL_VERTEX_SHADER, "./Resources/shadowShaders/vertexShadowShader.glsl");
		_shader.addShader(GL_FRAGMENT_SHADER, "./Resources/shadowShaders/fragmentShadowShader.glsl");
		_shader.addAttribute("inPosition", VERTICES);
		_shader.addAttribute("inTexcoord", TEXTURES);
		_shader.addAttribute("inNormal", NORMALS);
		_shader.addAttribute("inColor", COLORS);
		_shader.addUniform("ModelMatrix");

		_shader.addUniform("campfireLSM1");
		_shader.addUniform("campfireLSM2");
		_shader.addUniform("campfireLSM3");
		_shader.addUniform("campfireLSM4");

		_shader.addUniform("campfireSM1");
		_shader.addUniform("campfireSM2");
		_shader.addUniform("campfireSM3");
		_shader.addUniform("campfireSM4");

		_shader.addUniform("LightPosition");
		_shader.addUniform("LightColor");
		_shader.addUniform("EyePosition");
		_shader.addUbo("CameraMatrices", UBO_BP);
		_shader.create();

		_shaderP.addShader(GL_VERTEX_SHADER, "./Resources/particleShaders/particles-vs.glsl");
		_shaderP.addShader(GL_FRAGMENT_SHADER, "./Resources/particleShaders/fire-fs.glsl");
		_shaderP.addAttribute("in_vertex", 0);
		_shaderP.addAttribute("in_texCoord", 1);
		_shaderP.addAttribute("in_pos", 2);
		_shaderP.addAttribute("in_color", 3);
		_shaderP.addAttribute("in_size", 4);
		_shaderP.addAttribute("in_rot", 5);
		_shaderP.addUniform("ModelMatrix");
		_shaderP.addUniform("in_texture");
		_shaderP.addUniform("in_dissolveMap");
		_shaderP.addUbo("SharedMatrices", UBO_BP);
		_shaderP.create();
		_shaderP.bind();
		glUniform1i(_shaderP.getUniform("in_texture"), 0);
		glUniform1i(_shaderP.getUniform("in_dissolveMap"), 1);
		_shaderP.unbind();
	}


	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;

		auto camP = new avt::PerspectiveCamera(45.f, aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f));
		auto camO = new avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 20.f));
		camP->setSpeed(12.f);
		camO->setSpeed(12.f);

		_cams.add("per", camP);
		_cams.add("ort", camO);

	}

	void createShadows(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;
		/*
		_shadow = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 20.f)));
		_shadow.setPosition({ 4.0f, 0.0f, 0.0f });
		_shadow.lookAt({ 0.0f, 0.0f, 0.0f });
		_shadow.setup();
		*/
		/*
		_shadow = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f)));
		_shadow.setup();

		_shadow2 = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f)));
		_shadow2.setup();

		_shadow3 = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f)));
		_shadow3.setup();

		_shadow4 = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f)));
		_shadow4.setup();
		*/
		campfire.setupShadows();
	}

	void createLights() {
		//_lights.add("sun", new avt::Light({ 3.0f, 0.0f, -3.0f }, { 1.f, 0.5f, 0.f }));
		campfire = avt::PointLight({ 3.0f, 0.0f, -3.0f }, { 1.f, 0.5f, 0.f });
	}

	void createBloom(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		_bloom = new avt::Bloom();
		_bloom->create(winx, winy);
	}

public:

	MyApp() : avt::App() {}

	~MyApp() {}

	void initCallback(GLFWwindow* win) override {
		createCams(win);
		createLights();
		createShadows(win);
		createBloom(win);
		createShader();
		createScene();
	}

	void pollEventsCallback(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double dt) override {
		processKeyInput(win, dt);
		processMouseMovement(win, lastCursor, newCursor, dt);
	}

	void updateCallback(GLFWwindow* win, double dt) override {
		
		avt::Mat4 rotMat;

		_emitter->update(dt);
		
		if (_animating) {
			_time += dt;
			if (_time > _duration) {
				_time -= _duration;				
			}
		}
		
		if (_rotating) {
			_time2 += dt;

			float k = (float)_time2 / _duration2;
			_lightStruct->setRotation(avt::Quaternion({ 0,1.f,0.f }, k * 2 * avt::PI));
			_lightStruct->rotateZ(avt::PI / 10);

			campfire.setIntensity(_time2);
			if (_time2 > _duration2) {
				_time2 = 0;
				_rotating = false;
				_lightStruct->setRotation(avt::Quaternion({ 1.f,0,0 }, avt::PI/10));
				campfire.setIntensity(1.f);
			}
		}

		if (_morebloom) { //bloom
			_bloom->setBlurTex(1);
			_morebloom = false;
		}

		if (_lessbloom) { //bloom
			_bloom->setBlurTex(-1);
			_lessbloom = false;
		}
		if (_turnOffOnBloom) {
			_bloom->turnOffOnBloom();
			_turnOffOnBloom = false;
		}

		/*
		_lights.get("sun")->setPosition(_light->pos().to3D());

		_shadow.setPosition(_light->pos().to3D());
		_shadow.lookAt(_light->pos().to3D() + avt::Vector3(1.f, 0.0f, 0.0f));

		_shadow2.setPosition(_light->pos().to3D());
		_shadow2.lookAt(_light->pos().to3D() + avt::Vector3(0.0f, 0.0f, 1.f));

		_shadow3.setPosition(_light->pos().to3D());
		_shadow3.lookAt(_light->pos().to3D() + avt::Vector3(-1.f, 0.0f, 0.0f));

		_shadow4.setPosition(_light->pos().to3D());
		_shadow4.lookAt(_light->pos().to3D() + avt::Vector3(0.0f, 0.0f, -1.f));
		*/
		campfire.setPosition(_light->pos().to3D());
		_emitter->setTranslation(campfire.getPosition());
		//Update Shader uniforms
		_shader.bind();

		campfire.updateLightSpaceMatrices(_shader, _shader.getUniform("campfireLSM1"), _shader.getUniform("campfireLSM2"),
			_shader.getUniform("campfireLSM3"), _shader.getUniform("campfireLSM4"));

		campfire.updateLight(_shader, _shader.getUniform("LightPosition"), _shader.getUniform("LightColor"));

		avt::Vector3 camPos = _cams.get(_activeCam)->position();
		glUniform3f(_shader.getUniform("EyePosition"), camPos.x(), camPos.y(), camPos.z());
		_shader.unbind();
		
		
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);
		_renderer.clear();

		/*
		_shadow.renderToDepthMap(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);
		_shadow2.renderToDepthMap(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);
		_shadow3.renderToDepthMap(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);
		_shadow4.renderToDepthMap(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);

		_shader.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _shadow.depthMap());
		glUniform1i(_shader.getUniform("shadowMap"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _shadow2.depthMap());
		glUniform1i(_shader.getUniform("shadowMap2"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _shadow3.depthMap());
		glUniform1i(_shader.getUniform("shadowMap3"), 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, _shadow4.depthMap());
		glUniform1i(_shader.getUniform("shadowMap4"), 3);

		glActiveTexture(GL_TEXTURE0);
		_shader.unbind();
		*/

		campfire.renderShadowMaps(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);

		campfire.shadowMapTextureLoad(_shader, 0, _shader.getUniform("campfireSM1"), _shader.getUniform("campfireSM2"), _shader.getUniform("campfireSM3"), _shader.getUniform("campfireSM4"));

		renderWithBloom(win);
		//renderWithoutBloom(win);
	}

	void renderWithBloom(GLFWwindow* win) {
		_bloom->bindHDR();
		//_renderer.draw(_scene, _ub, _shader, _cams.get(_activeCam), _lights.get("sun"));
		_scene.draw(_ub, _cams.get(_activeCam), &campfire);
		//_scene.draw(_ub, _cams.get(_activeCam));
		avt::StencilPicker::getTargetOn(win); // stencil is lost after unbindHDR so this stores internally the pick
		_bloom->unbindHDR();

		_bloom->bindPingBlur();
		_bloom->renderHDR();
		_bloom->unbindPingBlur();

		_bloom->renderBlur();

		_bloom->renderBloomFinal();
	}

	void renderWithoutBloom(GLFWwindow* win) {
		//_renderer.draw(_scene, _ub, _shader, _cams.get(_activeCam), _lights.get("sun"));
		_scene.draw(_ub, _cams.get(_activeCam), &campfire);
		avt::StencilPicker::getTargetOn(win);
	}

	void windowResizeCallback(GLFWwindow* win, int w, int h) override {
		glViewport(0, 0, w, h);
		if (_cams.size() == 2) {
			_cams.get("ort")->resize(w, h);
			_cams.get("per")->resize(w, h);
		}
		_bloom->create(w,h);
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
			_morebloom = !_morebloom;
			break;
		case GLFW_KEY_L:
			_lessbloom = !_lessbloom;
			break;
		case GLFW_KEY_B:
			_turnOffOnBloom = !_turnOffOnBloom;
			break;
		case GLFW_KEY_X:
			_emitter->toggle();
			break;
		case GLFW_KEY_ENTER:
			_cloudSystem->regen();
			break;
		}

	}

	void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override {
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			double cursorX, cursorY;
			glfwGetCursorPos(win, &cursorX, &cursorY);

			int winx, winy;
			glfwGetWindowSize(win, &winx, &winy);
			int x = static_cast<int>(cursorX);
			int y = winy - static_cast<int>(cursorY);

			auto target = avt::StencilPicker::getLastPick();
			if (target.second == "fire") {
				_emitter->toggle();
				//_meshes.get("tree")->colorAll({ avt::random(), avt::random(), avt::random() });
				//_meshes.get("tree")->updateBufferData();
			}

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
	engine.setWindow(1280, 960, "Low Poly Loli", is_fullscreen, is_vsync);

	engine.init();
	engine.run();
	delete app;
	engine.shutdown();
	
	//exit(EXIT_SUCCESS);
	return 0;
}
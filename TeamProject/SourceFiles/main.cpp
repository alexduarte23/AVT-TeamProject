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
	avt::Shader _shader, _shaderFire, _shaderClouds, _shaderHUD;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene, _HUD;
	MyNodeCallback nodeCallback;

	avt::ParticleEmitter* _emitter = nullptr;

	avt::Shadow _shadow, _shadow2, _shadow3, _shadow4;
	avt::Bloom* _bloom = nullptr;

	avt::PointLight campfire;
	avt::DirectionalLight env;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;

	bool _cursorVisible = false;
	bool _fireOn = true;
	float _campfireBaseIntensity = 1.f;


	avt::SceneNode* _tree = nullptr, * _tree2 = nullptr, * _tree3 = nullptr, * _lightStruct = nullptr, * _light = nullptr, * _floor = nullptr, * _cloud = nullptr, * _floor2 = nullptr;
	std::vector<avt::Apple*> _apples;
	std::vector<avt::Bunny*> _bunny;
	avt::CloudSystem* _cloudSystem = nullptr;
	std::string _activeCam = "per";
	
	const float _duration = 3, _duration2 = 6;
	double _time = 0, _time2 = 0;
	bool _animating = true, _rotating = false, _selecting = false, _morebloom = false, _lessbloom = false, _turnOffOnBloom = false, _animatingApples = false, _isBunny = true;

	unsigned int _selected = -1; //stencil index of the currently selected scene node - mouse picking

	void createScene() {

		avt::StencilPicker::enable();

		auto treeM = _meshes.add("tree", new avt::Mesh("./Resources/Objects/treeNormal.obj"));
		treeM->colorAll({0.2f, 0.6f, 0.2f});
		treeM->setup();
		
		auto floorM = _meshes.add("floor", new avt::Mesh("./Resources/cube_vtn_flat.obj"));
		auto islandM = _meshes.add("island", new avt::Mesh("./Resources/Objects/newIsland2.obj"));
		islandM->setup();

		auto appleTreeIslandM = _meshes.add("appleTreeIsland", new avt::Mesh("./Resources/Objects/appleTreeIsland.obj"));
		appleTreeIslandM->setup();

		auto appleM = _meshes.add("apple", new avt::Mesh("./Resources/Objects/apple.obj"));
		appleM->setup();

		auto bunnyIslandM = _meshes.add("bunnyIsland", new avt::Mesh("./Resources/Objects/bunnyIsland.obj"));
		bunnyIslandM->setup();

		auto bushM = _meshes.add("bush", new avt::Mesh("./Resources/Objects/simplebush.obj"));
		bushM->setup();

		auto bunnyearLM = _meshes.add("bunnyLeftEar", new avt::Mesh("./Resources/Objects/leftear.obj"));
		bunnyearLM->setup();
		auto bunnyearRM = _meshes.add("bunnyRightEar", new avt::Mesh("./Resources/Objects/rightear.obj"));
		bunnyearRM->setup();
		auto bunnytailM = _meshes.add("bunnyTail", new avt::Mesh("./Resources/Objects/tail.obj"));
		bunnytailM->setup();

		auto fireplaceM = _meshes.add("fireplace", new avt::Mesh("./Resources/Objects/fireplace3.obj"));
		fireplaceM->setup();

		
		createAppleTree(appleTreeIslandM, appleM);
		
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

		_lightStruct = _scene.createNode();

		_light = _lightStruct->createNode();
		//_light->setTranslation(_lights.get("sun")->getPosition());
		_light->setTranslation(campfire.getPosition());
		//_lightStruct->setRotation(avt::Quaternion({ 0,0,1.f }, avt::PI/10));

		auto island = _scene.createNode(islandM);
		island->translate({ 5.f, -3.f, -10.5f });
		island->scale({ 1.f, 1.f, 1.f });

		auto bunnyIsland = _scene.createNode(bunnyIslandM);
		bunnyIsland->translate({ -10.5f, -4.5f, -8.5f });
		bunnyIsland->scale({ 2.2f, 2.2f, 2.2f });

		auto bush = new avt::Bunny();
		bush ->setMesh(bushM);
		bunnyIsland->addNode(bush);
		bush->translate({ 0.f,0.f,0.f });
		bush->setPosition({ 0.f,0.f,0.f });
		_bunny.push_back(bush);

		auto bunnyEarL = new avt::Bunny();
		bunnyEarL->setMesh(bunnyearLM);
		bunnyIsland->addNode(bunnyEarL);
		bunnyEarL->translate({ 0.f,-0.16f,-0.16f });
		bunnyEarL->setPosition({ 0.f,-0.16f,-0.16f });
		_bunny.push_back(bunnyEarL);

		auto bunnyEarR = new avt::Bunny();
		bunnyEarR->setMesh(bunnyearRM);
		bunnyIsland->addNode(bunnyEarR);
		bunnyEarR->translate({ 0.f,-0.16f,-0.16f });
		bunnyEarR->setPosition({ 0.f,-0.16f,-0.16f });
		_bunny.push_back(bunnyEarR);

		auto bunnyTail = new avt::Bunny();
		bunnyTail->setMesh(bunnytailM);
		bunnyIsland->addNode(bunnyTail);
		bunnyTail->translate({ -0.2f,0.f,0.f });
		bunnyTail->setPosition({ -0.2f,0.f,0.f });
		_bunny.push_back(bunnyTail);

		avt::StencilPicker::addTarget(bush, "bunny");

		//auto colorCube = _scene.createNode(colorCubeM);
		//colorCube->translate({ 0,0,5.f });
		//colorCube->scale({ .3f,.3f,.3f });

		//auto fireplace = _scene.createNode(fireplaceM);
		//fireplace->translate({ 2.7f,-1.35f,-5.f });
		//fireplace->scale({ .15f,.15f,.15f });
		//avt::StencilPicker::addTarget(fireplace, "fire");

		auto fireplace = island->createNode(fireplaceM);
		fireplace->translate({ -4.f,1.3f,6.5f });
		//colorCube->rotateY(-avt::PI/2);
		fireplace->scale({ .25f,.25f,.25f });
		avt::StencilPicker::addTarget(fireplace, "fire");

		_cloudSystem = new avt::CloudSystem();
		_scene.addNode(_cloudSystem);
		_cloudSystem->setShader(&_shaderClouds);
		_cloudSystem->translate({ 15.f,35.f,0 });
		_cloudSystem->scale({ 2.5f,2.5f,2.5f });

		_emitter = new avt::FireEmitter();
		avt::StencilPicker::addTarget(_emitter, "fire");
		_emitter->setShader(&_shaderFire);
		_emitter->scale({ .9f, .9f, .9f });
		_emitter->translate(campfire.getPosition() + avt::Vector3(0,-.5f,0));
		_scene.addNode(_emitter); // scene deletes nodes when destroyed

		env.setPosition(_light->pos().to3D() + avt::Vector3(0.0f, 10.f, 0.0f));


		_HUD.setShader(&_shaderHUD);

		auto crosshair = _HUD.addNode(new avt::HUDElement("Resources/textures/crosshair161.png"));
		crosshair->scale({ .5f, .5f, .5f });


#ifndef ERROR_CALLBACK
		avt::ErrorManager::checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
	}

	void createAppleTree(avt::Mesh* appleTreeIslandM, avt::Mesh* appleM) {
		auto appleTreeIsland = _scene.createNode(appleTreeIslandM);
		appleTreeIsland->translate({ 15.5f, -2.f, -8.5f });
		appleTreeIsland->scale({ 1.5f, 1.5f, 1.5f });

		avt::Apple* apple;
		std::vector<avt::Vector3> applePositions = { { 1.05f, 1.5f, 0.0f } , { 0.6f, 2.45f, 0.0f } , { 0.0f, 2.1f, 0.25f }, { -0.6f, 1.7f, 0.0f } };

		for (int i = 0; i < 4; i++) {
			apple = new avt::Apple();
			apple->setMesh(appleM);
			appleTreeIsland->addNode(apple);
			apple->setPosition(applePositions.at(i));
			apple->translate(applePositions.at(i));
			apple->scale({ 1.5f, 1.5f, 1.5f });

			_apples.push_back(apple);
			avt::StencilPicker::addTarget(apple, "apple" + std::to_string(i+1));
		}
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
		if (!_cursorVisible) { // free move
			_cams.get("ort")->processMouse(offset, dt, true);
			_cams.get("per")->processMouse(offset, dt, true);

		} else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { // drag move
			_cams.get("ort")->processMouse(offset, dt);
			_cams.get("per")->processMouse(offset, dt);

		}

	}

	void createShader() {

		// create regular mesh shader
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
		_shader.addUniform("campfireLSM5");

		_shader.addUniform("campfireSM1");
		_shader.addUniform("campfireSM2");
		_shader.addUniform("campfireSM3");
		_shader.addUniform("campfireSM4");
		_shader.addUniform("campfireSM5");

		_shader.addUniform("envLSM");
		_shader.addUniform("envSM");

		_shader.addUniform("campfirePos");
		_shader.addUniform("campfireColor");

		_shader.addUniform("envPos");
		_shader.addUniform("envColor");

		_shader.addUniform("EyePosition");
		_shader.addUbo("CameraMatrices", UBO_BP);
		_shader.create();


		// create clouds shader, simular to mesh shader but with instancing
		_shaderClouds.addShader(GL_VERTEX_SHADER, "./Resources/shadowShaders/vertexShadowCloudsShader.glsl");
		_shaderClouds.addShader(GL_FRAGMENT_SHADER, "./Resources/shadowShaders/fragmentShadowShader.glsl");
		_shaderClouds.addAttribute("inPosition", VERTICES);
		_shaderClouds.addAttribute("inTexcoord", TEXTURES);
		_shaderClouds.addAttribute("inNormal", NORMALS);
		_shaderClouds.addAttribute("inColor", COLORS);
		_shaderClouds.addAttribute("inOffset", 4);
		_shaderClouds.addAttribute("inSize", 5);
		_shaderClouds.addUniform("ModelMatrix");

		_shaderClouds.addUniform("campfireLSM1");
		_shaderClouds.addUniform("campfireLSM2");
		_shaderClouds.addUniform("campfireLSM3");
		_shaderClouds.addUniform("campfireLSM4");
		_shaderClouds.addUniform("campfireLSM5");

		_shaderClouds.addUniform("campfireSM1");
		_shaderClouds.addUniform("campfireSM2");
		_shaderClouds.addUniform("campfireSM3");
		_shaderClouds.addUniform("campfireSM4");
		_shaderClouds.addUniform("campfireSM5");

		_shaderClouds.addUniform("envLSM");
		_shaderClouds.addUniform("envSM");

		_shaderClouds.addUniform("campfirePos");
		_shaderClouds.addUniform("campfireColor");

		_shaderClouds.addUniform("envPos");
		_shaderClouds.addUniform("envColor");

		_shaderClouds.addUniform("EyePosition");
		_shaderClouds.addUbo("CameraMatrices", UBO_BP);
		_shaderClouds.create();


		// create fire shader
		_shaderFire.addShader(GL_VERTEX_SHADER, "./Resources/particleShaders/particles-vs.glsl");
		_shaderFire.addShader(GL_FRAGMENT_SHADER, "./Resources/particleShaders/fire-fs.glsl");
		_shaderFire.addAttribute("in_vertex", 0);
		_shaderFire.addAttribute("in_texCoord", 1);
		_shaderFire.addAttribute("in_pos", 2);
		_shaderFire.addAttribute("in_color", 3);
		_shaderFire.addAttribute("in_size", 4);
		_shaderFire.addAttribute("in_rot", 5);
		_shaderFire.addUniform("ModelMatrix");
		_shaderFire.addUniform("in_texture");
		_shaderFire.addUniform("in_dissolveMap");
		_shaderFire.addUbo("SharedMatrices", UBO_BP);
		_shaderFire.create();
		_shaderFire.bind();
		glUniform1i(_shaderFire.getUniform("in_texture"), 0);
		glUniform1i(_shaderFire.getUniform("in_dissolveMap"), 1);
		_shaderFire.unbind();


		// create hud shader
		_shaderHUD.addShader(GL_VERTEX_SHADER, "./Resources/HUDshaders/hud-vs.glsl");
		_shaderHUD.addShader(GL_FRAGMENT_SHADER, "./Resources/HUDshaders/hud-fs.glsl");
		_shaderHUD.addAttribute("inPosition", 0);
		_shaderHUD.addAttribute("inTexcoord", 1);
		_shaderHUD.addAttribute("inColor", 2);
		_shaderHUD.addUniform("ModelMatrix");
		_shaderHUD.addUniform("inTexture");
		_shaderHUD.addUbo("CameraMatrices", UBO_BP);
		_shaderHUD.create();
		_shaderHUD.bind();
		glUniform1i(_shaderHUD.getUniform("inTexture"), 0);
		_shaderHUD.unbind();
	}


	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;

		auto camP = new avt::PerspectiveCamera(60.f, aspect, 0.1f, 200.0f, avt::Vector3(0, 0, 10.f));
		auto camO = new avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 20.f));
		auto camHUD = new avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f));
		camP->setSpeed(8.f);
		camO->setSpeed(8.f);

		_cams.add("per", camP);
		_cams.add("ort", camO);
		_cams.add("HUD", camHUD);

	}

	void createShadows() {
		campfire.setupShadows();
		env.setupShadows();
	}

	void createLights() {
		campfire = avt::PointLight({ 1.f, -1.f, -3.9f }, { 1.f, 0.5f, 0.f });
		campfire.setIntensity(2.0f);
		env = avt::DirectionalLight({ 5.0f, 5.0f, 5.0f }, { 0.1f, 0.1f, 0.1f });
		env.setIntensity(0.8f);
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
		createShadows();
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
		_cloudSystem->update(dt);

		for(int i = 0; i < 4; i++)
			_apples.at(i)->animate();

		_bunny.at(0)->animateBush();
		_bunny.at(1)->animateLeftEar();
		_bunny.at(2)->animateRightEar();
		_bunny.at(3)->animateTail();
		
		if (_animating) {
			_time += dt;
			if (_fireOn && _campfireBaseIntensity != 2.0) {
				_campfireBaseIntensity = min(campfire.getIntensity() + (float)dt * 1.0f, 2.0f);
				campfire.setIntensity(_campfireBaseIntensity);
			}
			else if (_fireOn && _campfireBaseIntensity == 2.0){
				campfire.setIntensity(2.0f + 0.2f * (float)sin(_time * 15) + 0.2f * (float)sin(_time * 10));
			}
			else {
				_campfireBaseIntensity = max(campfire.getIntensity() - (float)dt * 1.0f, 0);
				campfire.setIntensity(_campfireBaseIntensity);
			}
			if (_time > _duration) {
				_time -= _duration;				
			}
		}
		
		//if (_rotating) {
		//	_time2 += dt;
		//	float k = (float)_time2 / _duration2;
		//	_lightStruct->setRotation(avt::Quaternion({ 0,1.f,0.f }, k * 2 * avt::PI));
		//	_lightStruct->rotateZ(avt::PI / 10);
		//	if (_time2 > _duration2) {
		//		_time2 = 0;
		//		_rotating = false;
		//		_lightStruct->setRotation(avt::Quaternion({ 1.f,0,0 }, avt::PI/10));
		//		campfire.setIntensity(1.f);
		//	}
		//}

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

		//campfire.setPosition(_light->pos().to3D());
		//_emitter->setTranslation(campfire.getPosition());

		//Update Shader uniforms

		campfire.updateLightSpaceMatrices(_shader, _shader.getUniform("campfireLSM1"), _shader.getUniform("campfireLSM2"),
			_shader.getUniform("campfireLSM3"), _shader.getUniform("campfireLSM4"), _shader.getUniform("campfireLSM5"));
		campfire.updateLightSpaceMatrices(_shaderClouds, _shaderClouds.getUniform("campfireLSM1"), _shaderClouds.getUniform("campfireLSM2"),
			_shaderClouds.getUniform("campfireLSM3"), _shaderClouds.getUniform("campfireLSM4"), _shaderClouds.getUniform("campfireLSM5"));

		campfire.updateLight(_shader, _shader.getUniform("campfirePos"), _shader.getUniform("campfireColor"));
		campfire.updateLight(_shaderClouds, _shaderClouds.getUniform("campfirePos"), _shaderClouds.getUniform("campfireColor"));

		env.updateLightSpaceMatrices(_shader, _shader.getUniform("envLSM"));
		env.updateLightSpaceMatrices(_shaderClouds, _shader.getUniform("envLSM"));

		env.updateLight(_shader, _shader.getUniform("envPos"), _shader.getUniform("envColor"));
		env.updateLight(_shaderClouds, _shaderClouds.getUniform("envPos"), _shaderClouds.getUniform("envColor"));

		avt::Vector3 camPos = _cams.get(_activeCam)->position();

		_shader.bind();
		glUniform3f(_shader.getUniform("EyePosition"), camPos.x(), camPos.y(), camPos.z());
		_shader.unbind();
		
		_shaderClouds.bind();
		glUniform3f(_shaderClouds.getUniform("EyePosition"), camPos.x(), camPos.y(), camPos.z());
		_shaderClouds.unbind();
		
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);
		_renderer.clear();

		campfire.renderShadowMaps(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);
		env.renderShadowMaps(_renderer, _scene, (unsigned int)winx, (unsigned int)winy);

		campfire.shadowMapTextureLoad(_shader, 0, _shader.getUniform("campfireSM1"), _shader.getUniform("campfireSM2"), _shader.getUniform("campfireSM3"), _shader.getUniform("campfireSM4"), _shader.getUniform("campfireSM5"));
		campfire.shadowMapTextureLoad(_shaderClouds, 0, _shaderClouds.getUniform("campfireSM1"), _shaderClouds.getUniform("campfireSM2"), _shaderClouds.getUniform("campfireSM3"), _shaderClouds.getUniform("campfireSM4"), _shaderClouds.getUniform("campfireSM5"));

		env.shadowMapTextureLoad(_shader, 5, _shader.getUniform("envSM"));
		env.shadowMapTextureLoad(_shaderClouds, 5, _shaderClouds.getUniform("envSM"));

		renderWithBloom(win);
		//renderWithoutBloom(win);

		_HUD.draw(_ub, _cams.get("HUD"), nullptr);
	}

	void renderWithBloom(GLFWwindow* win) {
		_bloom->bindHDR();
		_scene.draw(_ub, _cams.get(_activeCam), &campfire);

		// stencil is lost after unbindHDR so this stores internally the pick
		if (_cursorVisible) avt::StencilPicker::getTargetOnCursor(win);
		else				avt::StencilPicker::getTargetOnCenter(win);

		_bloom->unbindHDR();

		_bloom->bindPingBlur();
		_bloom->renderHDR();
		_bloom->unbindPingBlur();

		_bloom->renderBlur();

		_bloom->renderBloomFinal();
	}

	void renderWithoutBloom(GLFWwindow* win) {
		_scene.draw(_ub, _cams.get(_activeCam), &campfire);

		if (_cursorVisible) avt::StencilPicker::getTargetOnCursor(win);
		else				avt::StencilPicker::getTargetOnCenter(win);
	}

	void windowResizeCallback(GLFWwindow* win, int w, int h) override {
		glViewport(0, 0, w, h);
		if (_cams.size() == 3) {
			_cams.get("ort")->resize(w, h);
			_cams.get("per")->resize(w, h);
			_cams.get("HUD")->resize(w, h);
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
			_cursorVisible = !_cursorVisible;
			if (_cursorVisible) glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
			_fireOn = !_fireOn;
			_emitter->toggle();
			break;
		case GLFW_KEY_ENTER:
			_cloudSystem->createCloud();
			break;
		case GLFW_KEY_V:
			if (env.getIntensity() == .6f) {
				env.setColor({ 0.1f, 0.1f, 0.1f });
				env.setIntensity(0.3f);
			}
			else {
				env.setColor({ 1.f, 1.f, 0.3f });
				env.setIntensity(.6f);

			}
			break;
		}


	}

	void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double cursorX, cursorY;
			glfwGetCursorPos(win, &cursorX, &cursorY);

			int winx, winy;
			glfwGetWindowSize(win, &winx, &winy);
			int x = static_cast<int>(cursorX);
			int y = winy - static_cast<int>(cursorY);

			auto target = avt::StencilPicker::getLastPick();
			if (target.second == "fire") {
				_fireOn = !_fireOn;
				_emitter->toggle();
			}else if (target.second == "apple1") {
				_apples.at(0)->setAnimating();
			}else if (target.second == "apple2") {
				_apples.at(1)->setAnimating();
			}else if (target.second == "apple3") {
				_apples.at(2)->setAnimating();
			}else if (target.second == "apple4") {
				_apples.at(3)->setAnimating();
			}
			else if (target.second == "bunny") {
				_bunny.at(0)->setAnimating();
				_bunny.at(1)->setAnimating();
				_bunny.at(2)->setAnimating();
				_bunny.at(3)->setAnimating();
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
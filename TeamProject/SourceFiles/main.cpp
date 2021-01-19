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
	avt::Shader _shader, _shaderFire, _shaderParticles, _shaderClouds, _shaderHUD, _shaderBG;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene, _HUD;
	MyNodeCallback nodeCallback;


	avt::Shadow _shadow, _shadow2, _shadow3, _shadow4;
	avt::Bloom* _bloom = nullptr;

	avt::PointLight campfire;
	avt::DirectionalLight env;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;

	bool _cursorVisible = false;
	bool _fireOn = true;
	float _campfireBaseIntensity = 1.f;


	avt::ParticleEmitter* _fireEmitter = nullptr, * _dustEmitter = nullptr, * _fireflyEmitter = nullptr;
	avt::Background* _background = nullptr;
	avt::SceneNode* _crosshair = nullptr;
	avt::SceneNode* _tree = nullptr, * _tree2 = nullptr, * _tree3 = nullptr, * _lightStruct = nullptr, * _light = nullptr, * _floor = nullptr, * _cloud = nullptr, * _floor2 = nullptr;
	std::vector<avt::Apple*> _apples;
	std::vector<avt::Bunny*> _bunny;
	avt::CloudSystem* _cloudSystem = nullptr;
	std::string _activeCam = "per";

	avt::Vector3 ambient;
	float ambientStrength = 0.05f;
	bool _day = false;
	bool _dayTransition = false;
	float _transitionDuration = 1.f;
	float _transitionTime = 0;
	avt::Vector3 _dayColor = { 1.f, 1.f, 1.f };
	avt::Vector3 _dayBackColor = { .0916f, .631f, .58f };
	float _dayStrength = 0.15f;
	avt::Vector3 _nightColor = { 0.035f, 0.08f, 0.2f };
	avt::Vector3 _nightBackColor = { 0.0f, 0.0f, 0.001f };
	float _nightStrength = 0.05f;

	const float _duration = 100000, _duration2 = 6;
	double _time = 0, _time2 = 0, _time3 = 0;
	bool _animating = true, _rotating = false, _selecting = false, _morebloom = false, _lessbloom = false, _turnOffOnBloom = false, _animatingApples = false, _isBunny = true;

	unsigned int _selected = -1; //stencil index of the currently selected scene node - mouse picking

	void createScene() {

		avt::StencilPicker::enable();

		// MESHES -------------------
		/*
		auto treeM = _meshes.add("tree", new avt::Mesh("./Resources/Objects/treeNormal.obj"));
		treeM->colorAll({0.2f, 0.6f, 0.2f});
		treeM->setup();
		*/

		auto floorM = _meshes.add("floor", new avt::Mesh("./Resources/cube_vtn_flat.obj"));
		auto islandM = _meshes.add("island", new avt::Mesh("./Resources/Objects/finalIsland.obj"));
		islandM->setup();

		auto appleM = _meshes.add("apple", new avt::Mesh("./Resources/Objects/apple.obj"));
		appleM->setup();

		/*auto bunnyIslandM = _meshes.add("bunnyIsland", new avt::Mesh("./Resources/Objects/bunnyIsland.obj"));
		bunnyIslandM->setup();

		*/

		auto bushM = _meshes.add("bush", new avt::Mesh("./Resources/Objects/bush.obj"));
		bushM->setup();

		auto bunnyearLM = _meshes.add("bunnyLeftEar", new avt::Mesh("./Resources/Objects/leftear.obj"));
		bunnyearLM->setup();
		auto bunnyearRM = _meshes.add("bunnyRightEar", new avt::Mesh("./Resources/Objects/rightear.obj"));
		bunnyearRM->setup();
		auto bunnytailM = _meshes.add("bunnyTail", new avt::Mesh("./Resources/Objects/tail.obj"));
		bunnytailM->setup();

		auto fireplaceM = _meshes.add("fireplace", new avt::Mesh("./Resources/Objects/fireplace3.obj"));
		fireplaceM->setup();

		
		auto lightM = _meshes.add("moon", new avt::Mesh("./Resources/Objects/sun_moon.obj"));
		lightM->applyTransform(avt::Mat4::scale({ 0.5f, 0.5f, 0.5f }));
		lightM->colorAll({ 1.f, 1.f, 1.f });
		lightM->setup();

		auto colorCubeM = _meshes.add("colorCube", new avt::Mesh("./Resources/Objects/colourscube.obj"));
		colorCubeM->setup();

		auto tentM = _meshes.add("tent", new avt::Mesh("./Resources/Objects/tent.obj"));
		tentM->setup();

		//CAMS
		_ub.create(2 * 16 * sizeof(GLfloat), 0); // change
		_ub.unbind();

		_scene.setShader(&_shader);

		// SCENE ----------------------------
		_background = new avt::Background(_nightBackColor);
		_scene.addNode(_background);
		_background->setShader(&_shaderBG);

		_lightStruct = _scene.createNode();

		_light = _lightStruct->createNode();
		//_light->setTranslation(_lights.get("sun")->getPosition());
		_light->setTranslation(campfire.getPosition());
		//_lightStruct->setRotation(avt::Quaternion({ 0,0,1.f }, avt::PI/10));

		auto island = _scene.createNode(islandM);
		island->translate({ 5.f, -3.f, -10.5f });
		island->scale({ 1.f, 1.f, 1.f });

		createAppleTree(island, appleM);

		auto tent = island->createNode(tentM);
		avt::StencilPicker::addTarget(tent, "tent");

		createBunny(bushM, island, bunnyearLM, bunnyearRM, bunnytailM);

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

		_fireEmitter = new avt::FireEmitter();
		avt::StencilPicker::addTarget(_fireEmitter, "fire");
		_fireEmitter->setShader(&_shaderFire);
		_fireEmitter->scale({ .9f, .9f, .9f });
		_fireEmitter->translate(campfire.getPosition() + avt::Vector3(0,-.5f,0));
		_scene.addNode(_fireEmitter); // scene deletes nodes when destroyed

		_dustEmitter = new avt::DustEmitter(10, 5);
		_dustEmitter->setShader(&_shaderParticles);
		_dustEmitter->translate({10.f,-1.f,-17.f});
		_scene.addNode(_dustEmitter); // scene deletes nodes when destroyed

		_fireflyEmitter = new avt::FireflyEmitter(5, 2);
		_fireflyEmitter->setShader(&_shaderParticles);
		_fireflyEmitter->translate({ 24.f,8.f,-13.f });
		_scene.addNode(_fireflyEmitter); // scene deletes nodes when destroyed

		env.setPosition(_light->pos().to3D() - _light->pos().to3D() + avt::Vector3(6.0f, 25.f, 0.0f));


		_HUD.setShader(&_shaderHUD);

		_crosshair = _HUD.addNode(new avt::HUDElement("Resources/textures/crosshair161.png"));
		_crosshair->scale({ .5f, .5f, .5f });


#ifndef ERROR_CALLBACK
		avt::ErrorManager::checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
	}

	void createBunny(avt::Mesh* bushM, avt::SceneNode* island, avt::Mesh* bunnyearLM, avt::Mesh* bunnyearRM, avt::Mesh* bunnytailM)
	{

		auto bush = new avt::Bunny();
		bush->setMesh(bushM);
		island->addNode(bush);
		bush->translate({ 0.f,0.f,0.f });
		bush->setPosition({ 0.f,0.f,0.f });
		_bunny.push_back(bush);

		auto bunnyEarL = new avt::Bunny();
		bunnyEarL->setMesh(bunnyearLM);
		bush->addNode(bunnyEarL);
		//bunnyIsland->addNode(bunnyEarL);

		bunnyEarL->translate({ 0.f,-0.7f,0.7f });
		bunnyEarL->setPosition({ 0.f,-0.7f,0.7f });
		_bunny.push_back(bunnyEarL);

		auto bunnyEarR = new avt::Bunny();
		bunnyEarR->setMesh(bunnyearRM);
		bush->addNode(bunnyEarR);

		bunnyEarR->translate({ 0.f,-0.7f,0.7f });
		bunnyEarR->setPosition({ 0.f,-0.7f,0.7f });
		_bunny.push_back(bunnyEarR);


		auto bunnyTail = new avt::Bunny();
		bunnyTail->setMesh(bunnytailM);
		bush->addNode(bunnyTail);

		bunnyTail->translate({ 0.4f,0.f,0.f });
		bunnyTail->setPosition({ 0.4f,0.f,0.f });
		_bunny.push_back(bunnyTail);

		avt::StencilPicker::addTarget(bush, "bunny");
	}

	void createAppleTree(avt::SceneNode * island, avt::Mesh* appleM) {

		avt::Apple* apple;
		std::vector<avt::Vector3> applePositions = { { -3.8f, -2.5f, -0.2f } , { -2.3f, -1.5f, 1.f } , { 0.3f, 0.f, 0.0f }, { 1.5f, -2.5f, 0.0f } }; //7 height
		std::vector<float> heights = {5.1f, 5.5f, 7.0f, 4.3f};
		for (int i = 0; i < 4; i++) {
			apple = new avt::Apple();
			apple->setMesh(appleM);
			island->addNode(apple);
			apple->setPosition(applePositions.at(i));
			apple->setHeight(heights.at(i));
			apple->translate( applePositions.at(i));
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

	void createShaders() {

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

		_shader.addUniform("AmbientColor");
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

		_shaderClouds.addUniform("AmbientColor");
		_shaderClouds.addUniform("EyePosition");
		_shaderClouds.addUbo("CameraMatrices", UBO_BP);
		_shaderClouds.create();


		// create fire shader
		_shaderParticles.addShader(GL_VERTEX_SHADER, "./Resources/particleShaders/particles-vs.glsl");
		_shaderParticles.addShader(GL_FRAGMENT_SHADER, "./Resources/particleShaders/particles-fs.glsl");
		_shaderParticles.addAttribute("in_vertex", 0);
		_shaderParticles.addAttribute("in_texCoord", 1);
		_shaderParticles.addAttribute("in_pos", 2);
		_shaderParticles.addAttribute("in_color", 3);
		_shaderParticles.addAttribute("in_size", 4);
		_shaderParticles.addAttribute("in_rot", 5);
		_shaderParticles.addUniform("ModelMatrix");
		_shaderParticles.addUniform("in_texture");
		_shaderParticles.addUbo("SharedMatrices", UBO_BP);
		_shaderParticles.create();
		_shaderParticles.bind();
		glUniform1i(_shaderParticles.getUniform("in_texture"), 0);
		_shaderParticles.unbind();

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

		// create background shader
		_shaderBG.addShader(GL_VERTEX_SHADER, "./Resources/bgShaders/bg-vs.glsl");
		_shaderBG.addShader(GL_FRAGMENT_SHADER, "./Resources/bgShaders/bg-fs.glsl");
		_shaderBG.addAttribute("inPosition", 0);
		_shaderBG.addAttribute("inColor", 1);
		_shaderBG.create();
	}


	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;

		auto camP = new avt::PerspectiveCamera(60.f, aspect, 0.1f, 200.0f, avt::Vector3(0, 5.f, 10.f));
		auto camO = new avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 20.f));
		auto camHUD = new avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f));
		camP->lookAt(campfire.getPosition());
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
		env = avt::DirectionalLight({0.0f, 50.0f, 0.0f }, _nightColor);
		env.setIntensity(_nightStrength);
		ambient = _nightColor;
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
		createLights();
		createCams(win);
		createShadows();
		createBloom(win);
		createShaders();
		createScene();
	}

	void pollEventsCallback(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double dt) override {
		processKeyInput(win, dt);
		processMouseMovement(win, lastCursor, newCursor, dt);
	}

	void updateCallback(GLFWwindow* win, double dt) override {
		
		_time += dt;

		_fireEmitter->update(dt);
		_dustEmitter->update(dt);
		_fireflyEmitter->update(dt);
		_cloudSystem->update(dt);

		for(int i = 0; i < 4; i++)
			_apples.at(i)->animate((float)dt);

		_bunny.at(0)->animateBush();
		_bunny.at(1)->animateLeftEar();
		_bunny.at(2)->animateRightEar();
		_bunny.at(3)->animateTail();
		

		float speed = 2.0f;
		if (_fireOn && _campfireBaseIntensity != 2.0) {
			_campfireBaseIntensity = min(campfire.getIntensity() + (float)dt * speed, 2.0f);
			campfire.setIntensity(_campfireBaseIntensity);
		}
		else if (_fireOn && _campfireBaseIntensity == 2.0) {
			campfire.setIntensity(2.0f + 0.2f * (float)sin(_time * 15) + 0.2f * (float)sin(_time * 10));
		}
		else {
			_campfireBaseIntensity = max(campfire.getIntensity() - (float)dt * speed, 0);
			campfire.setIntensity(_campfireBaseIntensity);
		}
		if (_time > _duration) {
			_time = 0;
		}

		if (_dayTransition) {
			_transitionTime += (float)dt;
			float p = clamp(_transitionTime / _transitionDuration, 0, 1);
			float k =  _day ? p : 1.f - p;
			if (_transitionTime >= _transitionDuration) {
				_transitionTime = 0;
				_dayTransition = false;
			}
			transitionLight(k);
		}

		if (avt::StencilPicker::getLastPick().first == nullptr && avt::StencilPicker::getLastPick().second == "") {
			_crosshair->setScale({ .5f, .5f, .5f });
		}
		else {
			_crosshair->setScale({ .8f, .8f, .8f });

		}
		/*
		if (_rotating) {
			_time2 += dt;
			float k = (float)_time2 / _duration2;
			_lightStruct->setRotation(avt::Quaternion({ 0,1.f,0.f }, k * 2 * avt::PI));
			_lightStruct->rotateZ(avt::PI / 10);
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
		*/
		//campfire.setPosition(_light->pos().to3D());
		//_fireEmitter->setTranslation(campfire.getPosition());

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
		glUniform3f(_shader.getUniform("AmbientColor"), ambient.x()*ambientStrength, ambient.y() * ambientStrength, ambient.z() * ambientStrength);
		_shader.unbind();
		
		_shaderClouds.bind();
		glUniform3f(_shaderClouds.getUniform("EyePosition"), camPos.x(), camPos.y(), camPos.z());
		glUniform3f(_shaderClouds.getUniform("AmbientColor"), ambient.x() * ambientStrength, ambient.y() * ambientStrength, ambient.z() * ambientStrength);
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

	void transitionLight(float k) {
		env.setColor(k * _dayColor + (1.f - k) * _nightColor);
		env.setIntensity((k * _dayStrength + (1.f - k) * _nightStrength) * 5.f);
		ambient = k * _dayColor + (1.f - k) * _nightColor;
		ambientStrength = k * 2*_dayStrength + (1.f - k) * _nightStrength;
		_background->setColor(k * _dayBackColor + (1.f - k) * _nightBackColor);
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
		//case GLFW_KEY_P:
		//	_activeCam = _activeCam == "ort" ? "per" : "ort";
		//	break;
		case GLFW_KEY_ESCAPE:
			_cursorVisible = !_cursorVisible;
			if (_cursorVisible) glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		case GLFW_KEY_0:
			if (_cams.size() == 3) {
				_cams.get("ort")->setPosition(avt::Vector3(0, 5.f, 10.f));
				_cams.get("per")->setPosition(avt::Vector3(0, 5.f, 10.f));
				_cams.get("ort")->lookAt(avt::Vector3());
				_cams.get("per")->lookAt(campfire.getPosition());
			}
			break;
		/*
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
			_fireEmitter->toggle();
			break;
		case GLFW_KEY_ENTER:
			_cloudSystem->createCloud();
			break;
		case GLFW_KEY_V:
			_day = !_day;
			_dayTransition = true;
			break;
		*/
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
				_fireEmitter->toggle();
			}else if (target.second == "tent") {
				_day = !_day;
				_dayTransition = true;
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
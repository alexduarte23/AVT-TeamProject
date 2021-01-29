#pragma once
#include "avt_math.h"
#include "Globals.h"
#include "Shader.h"
#include "Camera.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Renderer.h"

namespace avt {

	class Shadow {
	private:
		unsigned int _shadowWidth;
		unsigned int _shadowHeight;
		unsigned int _depthMapFBO;
		unsigned int _depthMap;
		bool _rendered = false;
		Shader _depthShader;
		//OrthographicCamera _lightView;


		void genFrameBuffer() {
			glGenFramebuffers(1, &_depthMapFBO);
		}

		void createTexture() {
			glGenTextures(1, &_depthMap);
			glBindTexture(GL_TEXTURE_2D, _depthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				_shadowWidth, _shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		void attachTexture() {
			glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void createDepthShader() {
			_depthShader.addShader(GL_VERTEX_SHADER, "./Resources/shadowShaders/vertexDepthShader.glsl");
			_depthShader.addShader(GL_FRAGMENT_SHADER, "./Resources/shadowShaders/fragmentDepthShader.glsl");
			_depthShader.addAttribute("inPosition", VERTICES);
			_depthShader.addAttribute("inTexcoord", TEXTURES);
			_depthShader.addAttribute("inNormal", NORMALS);
			_depthShader.addAttribute("inColor", COLORS);
			_depthShader.addUniform("ModelMatrix");
			_depthShader.addUniform("lightSpaceMatrix");
			_depthShader.create();
		}

	public:

		/*
		OrthographicCamera _lightView;

		Shadow() :
			_depthMap(0), _depthMapFBO(0), _shadowWidth(1), _shadowHeight(1), _lightView(avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / 1, 10.0f / 1, 0.1f, 100.0f, avt::Vector3(0, 0, 20.f))) {}

		Shadow(unsigned int shadowWidth, unsigned int shadowHeight, avt::OrthographicCamera camera) :
			_depthMap(0), _depthMapFBO(0), _shadowWidth(shadowWidth), _shadowHeight(shadowHeight), _lightView(camera) {}
		*/

		Camera _lightView;

		Shadow() :
			_depthMap(0), _depthMapFBO(0), _shadowWidth(1), _shadowHeight(1), _lightView(avt::PerspectiveCamera(45.f, 1.f, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f))) {}

		Shadow(unsigned int shadowWidth, unsigned int shadowHeight, avt::Camera camera) :
			_depthMap(0), _depthMapFBO(0), _shadowWidth(shadowWidth), _shadowHeight(shadowHeight), _lightView(camera) {}

		~Shadow() {}

		void setup() {
			genFrameBuffer();
			createTexture();
			attachTexture();
			createDepthShader();
		}

		void renderToDepthMap(Renderer& renderer, Scene& scene, unsigned int screenWidth, unsigned int screenHeight) {
			if (_rendered) {
				return;
			}
			_depthShader.bind();
			glUniformMatrix4fv(_depthShader.getUniform("lightSpaceMatrix"), 1, GL_FALSE, (_lightView.projMatrix() * _lightView.viewMatrix()).GLdata());

			glViewport(0, 0, _shadowWidth, _shadowHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			renderer.draw(scene, _depthShader);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			_depthShader.unbind();

			glViewport(0, 0, screenWidth, screenHeight);
			renderer.clear();
			_rendered = true;
		}

		void setPosition(const Vector3& pos) {
			_lightView.setPosition(pos);
			_rendered = false;
		}

		void lookAt(const Vector3& target) {
			_lightView.lookAt(target);
			_rendered = false;
		}

		unsigned int depthMap() {
			return _depthMap;
		}

	};
}
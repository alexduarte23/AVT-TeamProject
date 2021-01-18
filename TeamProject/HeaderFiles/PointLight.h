#pragma once

#include "Light.h"
#include "Shadow.h"

namespace avt {

	class PointLight : public Light {
	public:
		Shadow shadowXplus, shadowZplus, shadowXminus, shadowZminus, shadowDown;

		PointLight() : Light() {
			shadowXplus = avt::Shadow();
			shadowZplus = avt::Shadow();
			shadowXminus = avt::Shadow();
			shadowZminus = avt::Shadow();
			shadowDown = avt::Shadow();
		}

		PointLight(avt::Vector3 pos, avt::Vector3 color) : Light(pos, color) {
			shadowXplus = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, 1.f, 0.1f, 100.0f, pos));
			shadowZplus = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, 1.f, 0.1f, 100.0f, pos));
			shadowXminus = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, 1.f, 0.1f, 100.0f, pos));
			shadowZminus = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, 1.f, 0.1f, 100.0f, pos));
			shadowDown = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::PerspectiveCamera(90.f, 1.f, 0.1f, 100.0f, pos));

			shadowXplus.lookAt(pos + avt::Vector3(1.f, 0.0f, 0.0f));
			shadowZplus.lookAt(pos + avt::Vector3(0.0f, 0.0f, 1.f));
			shadowXminus.lookAt(pos + avt::Vector3(-1.f, 0.0f, 0.0f));
			shadowZminus.lookAt(pos + avt::Vector3(0.0f, 0.0f, -1.f));
			shadowDown.lookAt(pos + avt::Vector3(0.1f, -100.f, 0.0f));
		}

		~PointLight() {}

		void setupShadows() {
			shadowXplus.setup();
			shadowXminus.setup();
			shadowZplus.setup();
			shadowZminus.setup();
			shadowDown.setup();

		}

		void setPosition(avt::Vector3 pos) {
			_pos = pos;
			shadowXplus.setPosition(pos);
			shadowXminus.setPosition(pos);
			shadowZplus.setPosition(pos);
			shadowZminus.setPosition(pos);
			shadowDown.setPosition(pos);
		}

		void updateLightSpaceMatrices(Shader& shader, GLuint xplus, GLuint xminus, GLuint zplus, GLuint zminus, GLuint down) {
			shader.bind();
			glUniformMatrix4fv(xplus, 1, GL_FALSE, (shadowXplus._lightView.projMatrix() * shadowXplus._lightView.viewMatrix()).GLdata());
			glUniformMatrix4fv(xminus, 1, GL_FALSE, (shadowXminus._lightView.projMatrix() * shadowXminus._lightView.viewMatrix()).GLdata());
			glUniformMatrix4fv(zplus, 1, GL_FALSE, (shadowZplus._lightView.projMatrix() * shadowZplus._lightView.viewMatrix()).GLdata());
			glUniformMatrix4fv(zminus, 1, GL_FALSE, (shadowZminus._lightView.projMatrix() * shadowZminus._lightView.viewMatrix()).GLdata());
			glUniformMatrix4fv(down, 1, GL_FALSE, (shadowDown._lightView.projMatrix() * shadowDown._lightView.viewMatrix()).GLdata());
			shader.unbind();
		}

		void renderShadowMaps(Renderer& renderer, Scene& scene, unsigned int screenWidth, unsigned int screenHeight) {
			shadowXplus.renderToDepthMap(renderer, scene, screenWidth, screenHeight);
			shadowZplus.renderToDepthMap(renderer, scene, screenWidth, screenHeight);
			shadowXminus.renderToDepthMap(renderer, scene, screenWidth, screenHeight);
			shadowZminus.renderToDepthMap(renderer, scene, screenWidth, screenHeight);
			shadowDown.renderToDepthMap(renderer, scene, screenWidth, screenHeight);
		}

		void shadowMapTextureLoad(Shader& shader, int firstTexture, GLuint xplus, GLuint xminus, GLuint zplus, GLuint zminus, GLuint down) {
			shader.bind();
			glActiveTexture(GL_TEXTURE0 + firstTexture);
			glBindTexture(GL_TEXTURE_2D, shadowXplus.depthMap());
			glUniform1i(xplus, firstTexture);

			glActiveTexture(GL_TEXTURE1 + firstTexture);
			glBindTexture(GL_TEXTURE_2D, shadowXminus.depthMap());
			glUniform1i(xminus, firstTexture + 1);

			glActiveTexture(GL_TEXTURE2 + firstTexture);
			glBindTexture(GL_TEXTURE_2D, shadowZplus.depthMap());
			glUniform1i(zplus, firstTexture + 2);

			glActiveTexture(GL_TEXTURE3 + firstTexture);
			glBindTexture(GL_TEXTURE_2D, shadowZminus.depthMap());
			glUniform1i(zminus, firstTexture + 3);

			glActiveTexture(GL_TEXTURE4 + firstTexture);
			glBindTexture(GL_TEXTURE_2D, shadowDown.depthMap());
			glUniform1i(down, firstTexture + 4);

			glActiveTexture(GL_TEXTURE0);
			shader.unbind();
		}
	};
}
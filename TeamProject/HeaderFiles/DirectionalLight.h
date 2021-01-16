#pragma once

#include "Light.h"
#include "Shadow.h"

namespace avt {

	class DirectionalLight : public Light {
	public:
		Shadow shadow;

		DirectionalLight() : Light() {
			shadow = avt::Shadow();
		}

		DirectionalLight(avt::Vector3 pos, avt::Vector3 color) : Light(pos, color) {
			shadow = avt::Shadow((unsigned int)1024, (unsigned int)1024, avt::OrthographicCamera(-10.0f, 10.0f, -10.0f, 20.0f, 0.1f, 100.0f));
			shadow.setPosition(pos);
		}

		~DirectionalLight() {}

		void setupShadows() {
			shadow.setup();
		}

		void setPosition(avt::Vector3 pos) {
			_pos = pos;
			shadow.setPosition(_pos);
			shadow.lookAt(avt::Vector3(0.0f, 0.0f, 0.0f));
		}

		void updateLightSpaceMatrices(Shader& shader, GLuint lsmuniform) {
			shader.bind();
			glUniformMatrix4fv(lsmuniform, 1, GL_FALSE, (shadow._lightView.projMatrix() * shadow._lightView.viewMatrix()).GLdata());
			shader.unbind();
		}

		void renderShadowMaps(Renderer& renderer, Scene& scene, unsigned int screenWidth, unsigned int screenHeight) {
			shadow.renderToDepthMap(renderer, scene, screenWidth, screenHeight);
		}

		void shadowMapTextureLoad(Shader& shader, int textureUnit, GLuint shadowUniform) {
			shader.bind();
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, shadow.depthMap());
			glUniform1i(shadowUniform, textureUnit);

			glActiveTexture(GL_TEXTURE0);
			shader.unbind();
		}
	};
}

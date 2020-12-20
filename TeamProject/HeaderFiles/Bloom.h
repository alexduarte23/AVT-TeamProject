#pragma once
#include <GL\glew.h>
#include "avt_math.h"
#include "Shader.h"
#include "Texture.h"
#include "Globals.h"

namespace avt {

	class Bloom {
	private:
		RenderTargetTexture _HDR;
		RenderTargetTexture _pingBlur;
		RenderTargetTexture _pongBlur;

		Shader _ShaderBrightValues;
		Shader _ShaderGaussianBlur;
		Shader _BloomFinal;

		void createShaders() {
			createShaderBrightValues();
			createShaderGaussianBlur();
			//createShaderBloomFinal();
		}

		void createShaderBrightValues() {
			_ShaderBrightValues.addShader(GL_VERTEX_SHADER, "./Resources/brightVertexshader.shader");
			_ShaderBrightValues.addShader(GL_FRAGMENT_SHADER, "./Resources/brightFragmentshader.shader");
			_ShaderBrightValues.addAttribute("inVertex", VERTICES); 
			_ShaderBrightValues.addAttribute("inTexcoord", TEXTURES); 
			_ShaderBrightValues.addUniform("TexFramebuffer"); 
			_ShaderBrightValues.create();
		}

		void createShaderGaussianBlur() {
			_ShaderGaussianBlur.addShader(GL_VERTEX_SHADER, "./Resources/gaussianblurVertexshader.shader");
			_ShaderGaussianBlur.addShader(GL_FRAGMENT_SHADER, "./Resources/gaussianblurFragmentshader.shader");
			_ShaderGaussianBlur.addAttribute("inVertex", VERTICES); 
			_ShaderBrightValues.addAttribute("inTexcoord", TEXTURES);
			_ShaderGaussianBlur.addUniform("TexFramebuffer");
			_ShaderGaussianBlur.addUniform("horizontal");
			_ShaderGaussianBlur.create();
		}

		void createShaderBloomFinal() {
			_ShaderGaussianBlur.addShader(GL_VERTEX_SHADER, "./Resources/bloomFinalVertexshader.shader");
			_ShaderGaussianBlur.addShader(GL_FRAGMENT_SHADER, "./Resources/bloomFinalFragmentshader.shader");
			_ShaderGaussianBlur.addAttribute("inVertex", VERTICES);
			_ShaderGaussianBlur.addUniform("TexFramebuffer");
			_ShaderGaussianBlur.addUniform("horizontal");
			_ShaderGaussianBlur.create();
		}

		void setDirectionBlur(bool horizontal) {
			_ShaderGaussianBlur.bind();
			glUniform1i(_ShaderGaussianBlur.getUniform("horizontal"), horizontal);
			_ShaderGaussianBlur.unbind();
		}

	public:
		void create(int width, int height) {

			//Framebuffers
			_HDR.create(width, height);
			_pingBlur.create(width, height);
			_pongBlur.create(width, height);
			//Shaders
			createShaders();
		}

		void renderHDR() {
			_HDR.renderQuad(&_ShaderBrightValues, "TexFramebuffer");
		}

		void renderBlur() {
			bool horizontal = true;
			unsigned int amount = 10;

			for (unsigned int i = 0; i < amount; i++)
			{
				_pongBlur.bindFramebuffer();
				setDirectionBlur(horizontal);
				_pingBlur.renderQuad(&_ShaderGaussianBlur, "TexFramebuffer");
				_pongBlur.unbindFramebuffer();
				horizontal = !horizontal;

				_pingBlur.bindFramebuffer();
				setDirectionBlur(horizontal);
				_pongBlur.renderQuad(&_ShaderGaussianBlur, "TexFramebuffer");
				_pingBlur.unbindFramebuffer();
				horizontal = !horizontal;

			}
			_pingBlur.renderQuad(&_ShaderGaussianBlur, "TexFramebuffer");
		}

		void bindHDR() {
			_HDR.bindFramebuffer();
		}

		void unbindHDR() {
			_HDR.unbindFramebuffer();
		}

		void bindPingBlur() {
			_pingBlur.bindFramebuffer();
		}

		void unbindPingBlur() {
			_pingBlur.unbindFramebuffer();
		}

		void bindPongBlur() {
			_pongBlur.bindFramebuffer();
		}

		void unbindPongBlur() {
			_pongBlur.unbindFramebuffer();
		}
	

	};
}

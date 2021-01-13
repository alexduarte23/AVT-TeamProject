#pragma once
#include <GL\glew.h>
#include "VertexArray.h"
#include "avt_math.h"
#include "Shader.h"

#include "../Dependencies/stb_image.h"
#include <string>

namespace avt {

	class Texture;
	class RenderTargetTexture;
	class MultipleRenderTarget;
	class Quad2D;



	/*class Texture {
	protected:
		GLuint id;
	public:
		Texture() : id(-1) {}
		virtual void bind();
		virtual void unbind();
	};*/

	class Texture {
	private:
		static std::unique_ptr<Texture> _default;

	protected:
		GLuint _texId;
		//unsigned int _texId;
		int _width, _height, _nrChannels;

		GLenum _wrap[2] = { 0,0 };
		GLenum _filter[2] = { 0,0 };
		bool _mipmap = false;

	public:
		Texture()
			: _texId(0), _width(0), _height(0), _nrChannels(0) {}

		~Texture() {
			if (_texId) {
				glBindTexture(GL_TEXTURE_2D, 0);
				glDeleteTextures(1, &_texId);
			}
		}

		void setWrap(GLenum wrapS, GLenum wrapT) {
			_wrap[0] = wrapS;
			_wrap[1] = wrapT;
		}

		void setFilter(GLenum filterMin, GLenum filterMag) {
			_filter[0] = filterMin;
			_filter[1] = filterMag;
		}

		void useMipmap() {
			_mipmap = true;
		}

		void create(const std::string& filename) {
			if (_texId) {
				unbind();
				glDeleteTextures(1, &_texId);
			}

			glGenTextures(1, &_texId);
			bind();

			if (_wrap[0]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap[0]);
			if (_wrap[1]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			if (_filter[0]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter[0]);
			if (_filter[1]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter[1]);

			unsigned char* data = stbi_load(filename.data(), &_width, &_height, &_nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, _nrChannels == 4 ? GL_RGBA : GL_RGB, _width, _height, 0, _nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
				if (_mipmap) glGenerateMipmap(GL_TEXTURE_2D);
			}
			else { // use error manager
				std::cout << "Failed to load texture" << std::endl;
			}

			stbi_image_free(data);
		}

		virtual void bind(unsigned int slot = 0) const {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, _texId);
		}

		virtual void unbind(unsigned int slot = 0) const {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		static const Texture* getDefault() {
			if (!_default.get()) {
				_default.reset(new Texture());
				_default->setWrap(GL_REPEAT, GL_REPEAT);
				_default->setFilter(GL_NEAREST, GL_NEAREST);
				_default->create("Resources/textures/default.png");
			}
			return _default.get();
		}
	};


	class Quad2D : Texture {
	private:
		GLuint _vaoId;
		GLuint _vboId;
		GLfloat _vertices[16];
		void destroy();

	public:
		Quad2D();
		~Quad2D();
		void create();
		void draw();
	};

	class RenderTargetTexture : Texture {
	private:
		Quad2D _quad;
		GLuint _framebuffer, _rboDepthStencil;
		GLfloat _r, _g, _b, _a;
		void destroy();
		void createColorTexture(const int width, const int height);
		void createRenderbufferObject(const int width, const int height);

	public:
		RenderTargetTexture();
		~RenderTargetTexture();
		//void bind() const override;
		//void unbind() const override;
		void create(const int width, const int height);
		void setFramebufferClearColor(const GLfloat e, const GLfloat g, const GLfloat b, const GLfloat a);
		void bindFramebuffer();
		void unbindFramebuffer();
		void renderQuad(Shader* shader, std::string textureUniform);
		void renderQuad(RenderTargetTexture rtt);
		GLuint getId() {
			return _texId;
		}
	};

	class MultipleRenderTarget : Texture {
	private:
		Quad2D _quad;
		GLuint _framebuffer, _rboDepthStencil;
		GLfloat _r, _g, _b, _a;
		GLuint colorBuffers[2];
		void destroy();
		void createColorTexture(const int width, const int height);
		void createRenderbufferObject(const int width, const int height);

	public:
		MultipleRenderTarget();
		~MultipleRenderTarget();
		void create(const int width, const int height);
		void bindFramebuffer();
		void unbindFramebuffer();
		void renderQuad(Shader* shader, std::string textureUniform);
		void renderAll(GLuint texture);
		GLuint getId() {
			return _texId;
		}
	};

}


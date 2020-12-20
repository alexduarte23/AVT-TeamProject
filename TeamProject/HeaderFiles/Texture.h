#pragma once
#include <GL\glew.h>
#include "VertexArray.h"
#include "avt_math.h"
#include "Shader.h"

namespace avt {

	class Texture;
	class RenderTargetTexture;
	class Quad2D;

	class Texture
	{
	protected:
		GLuint id;
	public:
		Texture() : id(-1) {}
		virtual void bind();
		virtual void unbind();
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
		void bind() override;
		void unbind() override;
		void create(const int width, const int height);
		void setFramebufferClearColor(const GLfloat e, const GLfloat g, const GLfloat b, const GLfloat a);
		void bindFramebuffer();
		void unbindFramebuffer();
		void renderQuad(Shader* shader, std::string textureUniform);
		void renderQuad(RenderTargetTexture rtt);
		GLuint getId() {
			return id;
		}
	};

}


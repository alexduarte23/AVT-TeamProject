#include "../HeaderFiles/Texture.h"
#include <cassert>
#include "../HeaderFiles/Globals.h"

void avt::RenderTargetTexture::destroy()
{

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &id);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &_rboDepthStencil);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &_framebuffer);
}

void avt::RenderTargetTexture::createColorTexture(const int width, const int height)
{

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
	
}

void avt::RenderTargetTexture::createRenderbufferObject(const int width, const int height)
{
	glGenRenderbuffers(1, &_rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rboDepthStencil);
}

avt::RenderTargetTexture::RenderTargetTexture() :

	_framebuffer(-1),
	_rboDepthStencil(-1),
	_r(0.0f), _g(0.0f), _b(0.0f), _a(1.0f)
{
}

avt::RenderTargetTexture::~RenderTargetTexture()
{
	destroy();
}

void avt::RenderTargetTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void avt::RenderTargetTexture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void avt::RenderTargetTexture::create(const int width, const int height)
{
	glGenFramebuffers(1, &_framebuffer);
	//std::cout << _framebuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	createColorTexture(width, height);
	createRenderbufferObject(width, height);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_quad.create();
}

void avt::RenderTargetTexture::setFramebufferClearColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
{
	_r = r; _g = g; _b = b; _a = a;
}

void avt::RenderTargetTexture::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}

void avt::RenderTargetTexture::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void avt::RenderTargetTexture::renderQuad(Shader* shader, std::string textureUniform) {

	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	glUniform1i(shader->getUniform(textureUniform), 0); //check, might be wrong
	

	glDisable(GL_DEPTH_TEST);
		//creates multiple times should create just once
	_quad.draw();
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);
	shader->unbind();
}

GLfloat data[] = { 1.0f, -1.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						-1.0f, -1.0f, 0.0f, 0.0f,
							-1.0f, 1.0f, 0.0f, 1.0f };

//Quad2D
avt::Quad2D::Quad2D() : _vaoId(-1), _vboId(-1)
{
	
	memcpy(_vertices, data, sizeof(data));
}

avt::Quad2D::~Quad2D()
{
	destroy();
}

void avt::Quad2D::destroy() {
	glDeleteBuffers(1, &_vboId);
	glDeleteVertexArrays(1, &_vaoId);

}

void avt::Quad2D::create() {
	
	/**
	_va.create();
	_vb.create(_vertices.GLdata(), 16 * sizeof(GLfloat));

	VertexBufferLayout layout;
	layout.add<GLfloat>(2); // VERTICES
	layout.add<GLfloat>(2); //TEXCOORDS
	_va.addBuffer(_vb, layout, 0);
	_va.unbind();
	/**/

	glGenVertexArrays(1, &_vaoId);
	glGenBuffers(1,  &_vboId);

	glBindVertexArray(_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(TEXTURES);
	glVertexAttribPointer(TEXTURES, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

}

void avt::Quad2D::draw() {
	glBindVertexArray(_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void avt::Texture::bind()
{
}

void avt::Texture::unbind()
{
}

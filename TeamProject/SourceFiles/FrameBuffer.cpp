#include "../HeaderFiles/FrameBuffer.h"
#include "../HeaderFiles/ErrorManager.h"


namespace avt {

	void FrameBuffer::create() {
		glGenFramebuffers(1, &_hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &_hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	}


	void FrameBuffer::unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
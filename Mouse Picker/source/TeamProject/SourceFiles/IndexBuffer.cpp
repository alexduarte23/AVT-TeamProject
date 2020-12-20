#include "..\HeaderFiles\IndexBuffer.h"

#include "..\HeaderFiles\ErrorManager.h"

namespace avt {

	void IndexBuffer::create(const void* data, GLuint count) {
		_count = count;
		glGenBuffers(1, &_iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &_iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not destroy Index Buffer.");
#endif
	}

	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
	}


	void IndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
#include "..\HeaderFiles\VertexBuffer.h"

#include "..\HeaderFiles\ErrorManager.h"


namespace avt {

	void VertexBuffer::create(const void* data, GLsizeiptr size) {
		glGenBuffers(1, &_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		_size = size;
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
		avt::ErrorManager::checkOpenGLError("ERROR: Could not destroy Vertex Buffer.");
#endif
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	}


	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::fill(const void* data, GLsizeiptr size) {
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}
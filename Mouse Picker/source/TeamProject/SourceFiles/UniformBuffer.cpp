#include "..\HeaderFiles\UniformBuffer.h"

#include "..\HeaderFiles\ErrorManager.h"


namespace avt {

	void UniformBuffer::create(GLsizeiptr size, const GLuint ubBinding) {
		glGenBuffers(1, &_uboID);
		glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, ubBinding, _uboID);
	}

	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &_uboID);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not destroy Uniform Buffer.");
#endif
	}

	void UniformBuffer::fill(std::initializer_list<Mat4> mList) {
		bind();
		GLintptr pos = 0;
		for (auto& m : mList) {
			glBufferSubData(GL_UNIFORM_BUFFER, pos, 16*sizeof(GLfloat), m.data());
			pos += 16 * sizeof(GLfloat);
		}
	}

	void UniformBuffer::bind() const {
		glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
	}


	void UniformBuffer::unbind() const {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

}
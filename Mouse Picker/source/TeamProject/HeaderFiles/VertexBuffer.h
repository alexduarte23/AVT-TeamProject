#pragma once

#include <GL/glew.h>

namespace avt{

	class VertexBuffer {
	private:
		GLuint _vboID;
	public:
		VertexBuffer() : _vboID(0) {}
		~VertexBuffer();

		void create(const void* data, GLsizeiptr size);

		void bind() const;

		void unbind() const;

		void fill(const void* data, GLsizeiptr size);
	};

}
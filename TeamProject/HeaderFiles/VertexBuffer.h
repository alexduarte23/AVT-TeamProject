#pragma once

#include <GL/glew.h>

namespace avt{

	class VertexBuffer {
	private:
		GLuint _vboID;
		GLsizei _size;
	public:
		VertexBuffer(): _vboID(0), _size(0) {}
		~VertexBuffer();

		void create(const void* data, GLsizeiptr size);

		void bind() const;

		void unbind() const;

		void fill(const void* data, GLsizeiptr size);

		GLsizei size() const {
			return _size;
		}
	};

}
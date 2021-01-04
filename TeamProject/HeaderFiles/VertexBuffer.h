#pragma once

#include <GL/glew.h>

namespace avt{

	class VertexBuffer {
	private:
		GLuint _vboID;
		GLsizeiptr _size, _loadedSize;
	public:
		VertexBuffer(): _vboID(0), _size(0), _loadedSize(0) {}
		~VertexBuffer();

		void create(const void* data, GLsizeiptr size);

		void bind() const;

		void unbind() const;

		void fill(const void* data, GLsizeiptr size);

		GLsizeiptr size() const {
			return _size;
		}
	};

}
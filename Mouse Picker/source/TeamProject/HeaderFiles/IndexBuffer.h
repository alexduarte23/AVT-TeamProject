#pragma once

#include <GL/glew.h>

namespace avt {

	class IndexBuffer {
	private:
		GLuint _iboID;
		unsigned int _count;
	public:
		IndexBuffer() : _iboID(0), _count(0) {}
		~IndexBuffer();

		void create(const void* data, GLuint count);

		void bind() const;

		void unbind() const;

		GLuint count() const {
			return _count;
		}
	};

}
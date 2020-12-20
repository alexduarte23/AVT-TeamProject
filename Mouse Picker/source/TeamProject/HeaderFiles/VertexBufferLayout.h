#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

namespace avt {

	struct LayoutEl {
		GLint count;
		GLenum type;
		GLuint typeSize;
		GLboolean norm;
	};

	class VertexBufferLayout {
	private:
		std::vector<LayoutEl> _elements;
		GLsizei _stride;
	public:
		VertexBufferLayout() : _stride(0) {}
		~VertexBufferLayout() {}

		template<typename T>
		void add(GLint count, GLboolean norm = GL_FALSE) {
			std::cerr << "layout type not supported" << std::endl;
		}

		template<>
		void add<GLfloat>(GLint count, GLboolean norm) {
			_elements.push_back({ count, GL_FLOAT, sizeof(GLfloat), norm });
			_stride += count * sizeof(GLfloat);
		}

		GLsizei stride() const {
			return _stride;
		}

		const std::vector<LayoutEl>& elements() const {
			return _elements;
		}
	};

}
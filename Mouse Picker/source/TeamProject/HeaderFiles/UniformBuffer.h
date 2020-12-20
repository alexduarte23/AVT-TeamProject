#pragma once

#include <GL/glew.h>
#include "Mat4.h"
#include <initializer_list>

namespace avt{

	class UniformBuffer {
	private:
		GLuint _uboID;
	public:
		UniformBuffer() : _uboID(0) {}
		~UniformBuffer();

		void create(GLsizeiptr size, const GLuint ubBinding);

		void fill(std::initializer_list<Mat4> mList);

		void bind() const;

		void unbind() const;
	};

}
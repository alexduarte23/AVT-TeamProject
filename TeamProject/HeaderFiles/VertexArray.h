#pragma once


#include <GL/glew.h>
#include "../HeaderFiles/VertexBuffer.h"
#include "../HeaderFiles/VertexBufferLayout.h"

namespace avt {

	class VertexArray {
	private:
		GLuint _vaoID;
		unsigned int _num;
	public:
		VertexArray() : _vaoID(0), _num(0) {}
		~VertexArray();

		void create();

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void bind() const;

		void unbind() const;
	};

}
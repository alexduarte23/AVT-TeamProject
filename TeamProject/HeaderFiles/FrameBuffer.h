#pragma once
#include <GL\glew.h>

namespace avt {

	class FrameBuffer
	{
	private:
		GLuint _hdrFBO;
	public:
		FrameBuffer() : _hdrFBO(0) {}
		~FrameBuffer();

		void create();

		void bind() const;

		void unbind() const;

	};

}
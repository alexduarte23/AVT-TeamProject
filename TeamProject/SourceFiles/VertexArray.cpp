 #include "..\HeaderFiles\VertexArray.h"

#include "..\HeaderFiles\ErrorManager.h"

namespace avt {

	void VertexArray::create() {
		glGenVertexArrays(1, &_vaoID);
		glBindVertexArray(_vaoID);
	}

	VertexArray::~VertexArray() {
		bind();
		for (unsigned int i = 0; i < _num; i++) {
			glDisableVertexAttribArray(i);
		}
		glDeleteVertexArrays(1, &_vaoID);

		unbind();
	}

	void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
		bind();
		vb.bind();

		long long offset = 0;
		for (int i = 0; i < layout.elements().size(); i++) {
			const auto& el = layout.elements()[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, el.count, el.type, el.norm, layout.stride(), (const void*)offset); // buffer index, num items, type, norm, stride to next, offset
			offset += (long long)el.count * el.typeSize;
		}

#ifndef ERROR_CALLBACK
		ErrorManager::checkOpenGLError("ERROR: Could not destroy Vertex Array.");
#endif
	}

	void VertexArray::bind() const {
		glBindVertexArray(_vaoID);
	}


	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

}
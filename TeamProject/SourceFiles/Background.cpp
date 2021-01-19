#include "../HeaderFiles/Background.h"

#include "../HeaderFiles/Shader.h"

namespace avt {

	const float Background::QUAD_STRIP[4 * 6] = { // pos tex color
			-1.f, -1.f, 0,    1.f, 1.f, 1.f,  // bottom left
			 1.f, -1.f, 0,    1.f, 1.f, 1.f,  // bottomm right
			-1.f,  1.f, 0,    1.f, 1.f, 1.f,  // top left
			 1.f,  1.f, 0,    1.f, 1.f, 1.f   // top right
	};


	Background::Background(const Vector3& color) {
		_va.create();

		VertexBufferLayout layout;
		layout.add<GLfloat>(3); // VERTICES
		layout.add<GLfloat>(3); // COLORS

		_vb.create(QUAD_STRIP, sizeof(QUAD_STRIP));
		_va.addBuffer(_vb, layout);

		_vb.unbind();
		_va.unbind();

	}

	void Background::draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
		auto newWorldMat = worldMatrix * getTransform();
		if (getShader()) {
			getShader()->bind();
		}

		_va.bind();

		beforeDraw();
		glDepthMask(GL_FALSE);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDepthMask(GL_TRUE);
		afterDraw();

		_va.unbind();

		if (getShader()) {
			getShader()->unbind();
			shader->bind();
		}

	}

}
#include "../HeaderFiles/Background.h"

#include "../HeaderFiles/Shader.h"

namespace avt {


	Background::Background(const Vector3& color) {
		for (int i = 0; i < 4*6; i += 6) {
			QUAD_STRIP[i + 3] = color.x();
			QUAD_STRIP[i + 4] = color.y();
			QUAD_STRIP[i + 5] = color.z();
		}

		_color = color;

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
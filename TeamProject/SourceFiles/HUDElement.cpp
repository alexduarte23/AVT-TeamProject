#include "../HeaderFiles/HUDElement.h"

namespace avt {

	const float HUDElement::QUAD_STRIP[4 * 9] = { // pos tex color
			-1.f, -1.f, 0,      0,   0,   1.f, 1.f, 1.f, 1.f, // bottom left
			 1.f, -1.f, 0,    1.f,   0,   1.f, 1.f, 1.f, 1.f, // bottomm right
			-1.f,  1.f, 0,      0, 1.f,   1.f, 1.f, 1.f, 1.f, // top left
			 1.f,  1.f, 0,    1.f, 1.f,   1.f, 1.f, 1.f, 1.f // top right
	};


	HUDElement::HUDElement(const std::string& textureFilename) {
		_sprite.setWrap(GL_REPEAT, GL_REPEAT);
		_sprite.setFilter(GL_LINEAR, GL_LINEAR);
		_sprite.create(textureFilename);

		_va.create();

		VertexBufferLayout layout;
		layout.add<GLfloat>(3); // VERTICES
		layout.add<GLfloat>(2); // TEXTURE COORDS
		layout.add<GLfloat>(4); // COLORS

		_vb.create(QUAD_STRIP, sizeof(QUAD_STRIP));
		_va.addBuffer(_vb, layout);

		_vb.unbind();
		_va.unbind();

	}

	void HUDElement::draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
		auto newWorldMat = worldMatrix * getTransform();
		Shader* curr_shader = getShader() ? getShader() : shader;
		if (getShader()) {
			getShader()->bind();
		}

		_va.bind();
		_sprite.bind();

		beforeDraw();
		glDisable(GL_DEPTH_TEST);
		glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glEnable(GL_DEPTH_TEST);
		afterDraw();

		_sprite.unbind();

		_va.unbind();

		if (getShader()) {
			getShader()->unbind();
			shader->bind();
		}

	}

}
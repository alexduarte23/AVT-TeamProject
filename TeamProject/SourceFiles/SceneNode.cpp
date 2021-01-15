#include "../HeaderFiles/SceneNode.h"

#include "../HeaderFiles/Mesh.h"
#include "../HeaderFiles/Light.h"
#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/StencilPicker.h"

namespace avt {

	void SceneNode::draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
		auto newWorldMat = worldMatrix * getTransform();
		Shader* curr_shader = _shader ? _shader : shader;
		if (_shader) {
			_shader->bind();
			/*
			glUniform3f(curr_shader->getUniform("LightPosition"), light->getPosition().x(), light->getPosition().y(), light->getPosition().z());
			glUniform3f(curr_shader->getUniform("LightColor"), light->getColor().x(), light->getColor().y(), light->getColor().z());
			*/
		}

		if (_mesh) {
			//curr_shader->bind();
			_mesh->va().bind();

			//glEnable(GL_STENCIL_TEST);
			//glStencilFunc(GL_ALWAYS, getStencilIndex(), 0xFF);
			//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			StencilPicker::prepareStencil(_stencilIndex);

			//glUniform3f(curr_shader->getUniform("LightPosition"), light->getPosition().x(), light->getPosition().y(), light->getPosition().z());
			//glUniform3f(curr_shader->getUniform("LightColor"), light->getColor().x(), light->getColor().y(), light->getColor().z());

			beforeDraw();
			glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
			glDrawArrays(GL_TRIANGLES, 0, _mesh->vb().size());
			afterDraw();

			//glDisable(GL_STENCIL_TEST);

			_mesh->va().unbind();
			//curr_shader->unbind();
		}

		for (auto childNode : _nodes) {
			childNode->draw(curr_shader, newWorldMat, light);
		}

		if (_shader) {
			_shader->unbind();
			shader->bind();
		}
	}

}
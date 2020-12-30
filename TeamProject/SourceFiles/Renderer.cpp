#include "../HeaderFiles/Renderer.h"

#include "../HeaderFiles/Mat4.h"
#include "../HeaderFiles/Mesh.h"

namespace avt {

	void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, UniformBuffer& ub, Shader& shader, Camera* camera) const {
		va.bind();
		ib.bind();
		ub.bind();
		shader.bind();

		ub.fill({ camera->viewMatrix(), camera->projMatrix()});

		glUniformMatrix4fv(shader.getUniform(MODEL_MATRIX), 1, GL_FALSE, Mat4::identity().data());
		glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_BYTE, (GLvoid*)0);

		shader.unbind();
		va.unbind();
		ib.unbind();
		ub.unbind();
	}

	void Renderer::draw(SceneNode* node, UniformBuffer& ub, Shader& shader, Camera* camera) {
		shader.bind();
		ub.bind();

		ub.fill({ camera->viewMatrix(), camera->projMatrix() });
		drawNode(node, shader, Mat4::identity());

		ub.unbind();ub.bind();
		shader.unbind();
	}

	void Renderer::draw(const Scene& scene, UniformBuffer& ub, Shader& shader, Camera* camera) {
		shader.bind();
		ub.bind();

		ub.fill({ camera->viewMatrix(), camera->projMatrix() });
		drawNode(scene.getRoot(), shader, Mat4::identity());

		ub.unbind();ub.bind();
		shader.unbind();
	}

	void Renderer::drawNode(SceneNode* node, Shader& shader, const Mat4& worldMatrix) {
		auto newWorldMat = worldMatrix * node->getTransform();

		enableStencilBuffer(node);

		if (node->getMesh()) {
			Mesh* mesh = node->getMesh();

			mesh->va().bind();
			mesh->ib().bind();

			if (node->isSelected())
				glUniform4f(shader.getUniform("Color"), 0.9f, 0.9f, 0.9f, 1.0f);
			else
				glUniform4f(shader.getUniform("Color"), (mesh->getVertices()[0].color).x(), (mesh->getVertices()[0].color).y(), (mesh->getVertices()[0].color).z(), (mesh->getVertices()[0].color).w());
			
			
			//node->beforeDraw();
			glUniformMatrix4fv(shader.getUniform(MODEL_MATRIX), 1 ,GL_FALSE, node->getTransform().data());
			
			glDrawElements(GL_TRIANGLES, mesh->ib().count(), GL_UNSIGNED_BYTE, (GLvoid*)0);
			//node->afterDraw();


			mesh->va().unbind();
			mesh->ib().unbind();
		}

		disableStencilBuffer();

		for (auto childNode : node->children()) {
			drawNode(childNode, shader, newWorldMat);
		}
	}

	//Stencil buffer Mouse picking
	void Renderer::disableStencilBuffer()
	{
		glDisable(GL_STENCIL_TEST);
	}

	void Renderer::enableStencilBuffer(avt::SceneNode* node)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, node->getStencilIndex(), 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}
	//


	void Renderer::clear() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
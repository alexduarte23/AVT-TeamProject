#pragma once


#include <GL/glew.h>
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

#include "Scene.h"
#include "SceneNode.h"

namespace avt {

	class Renderer {
	private:
		void drawNode(SceneNode* node, Shader& shader, const Mat4& worldMatrix);

	public:
		//Renderer (const void* data, GLuint count);
		Renderer() {}
		~Renderer() {}

		void draw(const VertexArray& va, const IndexBuffer& ib, UniformBuffer& ub, Shader& shader, Camera* camera) const;

		void draw(const Scene& scene, UniformBuffer& ub, Shader& shader, Camera* camera);
		void draw(SceneNode* node, UniformBuffer& ub, Shader& shader, Camera* camera);
		void draw(const Scene& scene, UniformBuffer& ub, Shader& shader, Camera* camera, Light* light);
		void draw(const Scene& scene, Shader& shader);
		void disableStencilBuffer();
		void enableStencilBuffer(avt::SceneNode* node);

		void clear() const;
	};

}
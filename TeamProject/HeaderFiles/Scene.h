#pragma once

#include "SceneNode.h"

namespace avt {

	class Scene {
	private:
		SceneNode* _root;
	public:
		Scene() : _root(new SceneNode()) {}
		~Scene() {
			delete _root;
		}

		SceneNode* createNode(Mesh* mesh = nullptr) {
			return _root->createNode(mesh);
		}

		SceneNode* getRoot() const {
			return _root;
		}

		SceneNode* addNode(SceneNode* node) {
			return _root->addNode(node);
		}

		void setShader(Shader* shader) {
			_root->setShader(shader);
		}

		Shader* getShader() {
			return _root->getShader();
		}

		void draw(UniformBuffer& ub, Camera* camera, Light* light) {
			ub.bind();

			ub.fill({ camera->viewMatrix(), camera->projMatrix() });
			_root->draw(Mat4::identity(), light);

			ub.unbind();
		}
	};

}
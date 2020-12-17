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

		SceneNode* addNode(SceneNode* node) {
			return _root->addNode(node);
		}

		SceneNode* getRoot() const {
			return _root;
		}

		void draw(UniformBuffer& ub, Camera* camera) {
			ub.bind();

			ub.fill({ camera->viewMatrix(), camera->projMatrix() });
			_root->draw(Mat4::identity());

			ub.unbind();ub.bind();
		}
	};

}
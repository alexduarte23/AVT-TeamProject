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
	};

}
#include "../HeaderFiles/StencilPicker.h"

#include <string>
#include "../HeaderFiles/SceneNode.h"


namespace avt {

	std::vector<std::pair<SceneNode*, std::string>> StencilPicker::_targets;


	bool StencilPicker::addTarget(SceneNode* target, const std::string& alias) {
		if (_targets.size() == MAX) return false;

		target->setStencilIndex((unsigned int)_targets.size() + 1);
		_targets.push_back({ target, alias });
		return true;
	}

	SceneNode* StencilPicker::removeTarget(SceneNode* target) {
		SceneNode* el = nullptr;

		for (size_t i = 0; i < _targets.size(); i++) {
			if (_targets[i].first == target) {
				el = _targets[i].first;
				el->setStencilIndex(0);
				_targets.erase(_targets.begin() + i);
				break;
			}
		}

		if (el) {
			for (size_t i = 0; i < _targets.size(); i++)
				_targets[i].first->setStencilIndex((unsigned int)i + 1);
		}

		return el;

		/*
		size_t rem_i = -1;

		for (size_t i = 0; i < _targets.size(); i++) {
			if (rem_i != -1)
				_targets[i].first->setStencilIndex(i);
			else if (_targets[i].first == target) {
				rem_i = i;
				_targets[i].first->setStencilIndex(0);
			}
		}
		if (rem_i < 0) return nullptr;

		SceneNode* el = _targets[rem_i].first;
		_targets.erase(_targets.begin() + rem_i);
		return el;
		*/
	}

	SceneNode* StencilPicker::removeTarget(const std::string& alias) {
		SceneNode* el = nullptr;

		for (size_t i = 0; i < _targets.size(); i++) {
			if (_targets[i].second == alias) {
				el = _targets[i].first;
				el->setStencilIndex(0);
				_targets.erase(_targets.begin() + i);
				break;
			}
		}

		if (el) {
			for (size_t i = 0; i < _targets.size(); i++)
				_targets[i].first->setStencilIndex((unsigned int)i + 1);
		}

		return el;
	}

	void StencilPicker::removeAllTargets(const std::string& alias) {
		SceneNode* el = nullptr;

		for (size_t i = 0; i < _targets.size(); ) {
			if (_targets[i].second == alias) {
				el = _targets[i].first;
				el->setStencilIndex(0);
				_targets.erase(_targets.begin() + i);
			}
			else {
				i++;
			}
		}

		if (el) {
			for (size_t i = 0; i < _targets.size(); i++)
				_targets[i].first->setStencilIndex((unsigned int)i + 1);
		}
	}

	void StencilPicker::clear() {
		for (auto& t : _targets)
			t.first->setStencilIndex(0);
		_targets.clear();
	}

	std::pair<SceneNode*, std::string> StencilPicker::getTargetOn(int x, int y) {
		GLuint index;
		glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
		if (index) return _targets[index - 1];
		return { nullptr, "" };
	}

}
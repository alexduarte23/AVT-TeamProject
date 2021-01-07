#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

namespace avt {
	class SceneNode;

	class StencilPicker {
	private:

		static constexpr unsigned short MAX = 256;

		static std::vector<std::pair<SceneNode*, std::string>> _targets;

		StencilPicker() {}

	public:

		~StencilPicker() {}

		static void enable() {
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}

		static void disable() {
			glDisable(GL_STENCIL_TEST);
		}
		
		static void prepareStencil(unsigned int index) {
			glStencilFunc(GL_ALWAYS, index, 0xFF);
		}

		static bool addTarget(SceneNode* target, const std::string& alias = "");

		static SceneNode* removeTarget(SceneNode* target);

		static SceneNode* removeTarget(const std::string& alias);

		static void removeAllTargets(const std::string& alias);

		static void clear();

		static std::pair<SceneNode*, std::string> getTargetOn(int x, int y);

	};
}
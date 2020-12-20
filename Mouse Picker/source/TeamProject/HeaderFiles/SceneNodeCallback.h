#pragma once

namespace avt {

	class SceneNodeCallback {
	public:
		SceneNodeCallback () {}
		virtual ~SceneNodeCallback () {}

		virtual void beforeDraw() {}
		virtual void afterDraw() {}

	};

}
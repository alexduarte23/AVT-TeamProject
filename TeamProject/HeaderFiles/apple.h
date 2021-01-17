#pragma once
#include "SceneNode.h"

namespace avt {

	class Apple : public SceneNode{

		bool _animating = false;
		avt::Vector3 _position = { 0,0,0 };
		int _time = 0;
	
	public:

		void animate() {
			if (_animating && _time < 4) {
				this->translate({ 0.0f, -_position.y() / 4, 0.0f });
				_time++;
			}
			else if (_animating && _time >= 4 && _time < 8) {
				this->rotate(avt::Quaternion(1.f, 1.f, 0.f, 2.f));
				this->translate({ 0.04f,0,0 });
				_time++;
			}
			else if (_animating && _time >= 8 && _time < 100)
				_time++;

			else if (_animating && _time == 100) {
				this->rotate(avt::Quaternion(1.f, 1.f, 0.f, -8.f));
				this->setTranslation(_position);
				_animating = false;
			}
		}

		void setAnimating() {
			if (_animating == false) {
				_animating = true;
				_time = 0;
			}
		}

		bool getAnimating() {
			return _animating;
		}

		void setPosition(avt::Vector3 p) {
			_position = p;
		}
	};
}
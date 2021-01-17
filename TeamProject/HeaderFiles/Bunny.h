#pragma once
#include "SceneNode.h"

namespace avt {

	class Bunny : public SceneNode {

		bool _animating = false;
		avt::Vector3 _position = { 0,0,0 };
		int _time = 0;

	public:

		void animateLeftEar() {
			if (_animating && _time < 8) {
				this->translate({ 0.0f, 0.02f, 0.02f });
				_time++;
			}
			else if (_animating && _time >= 8 && _time < 60)
				_time++;

			else if (_animating && _time == 60) {
				this->setTranslation(_position);
				_animating = false;
			}
		}

		void animateRightEar() {
			if (_animating && _time < 8) {
				this->translate({ 0.0f, 0.02f, 0.02f });
				_time++;
			}
			else if (_animating && _time >= 8 && _time < 60)
				_time++;

			else if (_animating && _time == 60) {
				this->setTranslation(_position);
				_animating = false;
			}
		}

		void animateTail() {
			if (_animating && _time < 4) {
				_time++;
			}
			else if (_animating && _time < 12) {
				this->translate({ 0.025f, 0.f, 0.f });
				_time++;
			}
			else if (_animating && _time >= 12 && _time < 60)
				_time++;

			else if (_animating && _time == 60) {
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
#pragma once
#include "SceneNode.h"

namespace avt {

	class Apple : public SceneNode{

		bool _animating = false;
		avt::Vector3 _position = { 0,0,0 };
		float _height = 0;
		float _time = 0;
	
	public:

		void animate(float dt) {
			if (_animating) {
				float duration = 0.4f;
				float duration2 = 0.6f;
				float duration3 = 3.f;

				float distance = 0.1f;
				if (_time + dt <= duration) {
					this->translate({ 0.0f, -(dt * _height) / duration, 0.0f });
					_time += dt;
				}
				else if (_time < duration) {
					float d = duration - _time;
					this->translate({ 0.0f, -(d * _height) / duration, 0.0f });
					_time += d;
				}
				/** /
				else if (_time >= duration && _time + dt <= duration2) {
					//this->setRotation(avt::Quaternion(1.f, 1.f, 0.f, -(dt * 8.f) / 0.2f));
					//this->translate({ -0.04f,0,0 });
					_time += dt;
				}
				else if (_time >= duration && _time < duration2) {
					float d = duration2 - _time;
					//this->setRotation(avt::Quaternion(1.f, 1.f, 0.f, -(d * 8.f) / 0.2f));
					//this->translate({ -0.04f,0,0 });
					_time += d;
				}
				
				else if (_animating && _time >= 4 && _time < 8) {
					this->rotate(avt::Quaternion(1.f, 1.f, 0.f, 2.f));
					this->translate({ -0.04f,0,0 });
					_time++;
				}
				/**/
				else if (_time >= duration && _time < duration3) {
					_time += dt;
				}
				else if (_time >= duration && _time >= duration3) {
					//this->rotate(avt::Quaternion(1.f, 1.f, 0.f, -8.f));
					this->setTranslation(_position);
					_animating = false;
				}
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

		void setHeight(float h) {
			_height = h;
		}
	};
}
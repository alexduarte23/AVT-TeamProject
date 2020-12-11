#pragma once
#include "avt_math.h"

namespace avt {
	class Light {
	private:
		avt::Vector3 _pos;
		avt::Vector3 _color;

	public:

		Light(): _pos({0.0f, 0.0f, 0.0f}), _color(1.f, 1.f, 1.f) {}

		Light(avt::Vector3 pos, avt::Vector3 color): _pos(pos), _color(color){}

		avt::Vector3 getPosition() {
			return _pos;
		}

		void setPosition(avt::Vector3 pos) {
			_pos = pos;
		}

		avt::Vector3 getColor() {
			return _color;
		}

		void setColor(avt::Vector3 color) {
			_color = color;
		}
	};
}
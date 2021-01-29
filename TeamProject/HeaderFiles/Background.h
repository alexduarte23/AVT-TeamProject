#pragma once

#include "SceneNode.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace avt {

	class Background : public SceneNode{
	private:

		VertexArray _va;
		VertexBuffer _vb;

		Vector3 _color;

		float QUAD_STRIP[4 * 6] = {
			-1.f, -1.f, 0,    1.f, 1.f, 1.f,  // bottom left
			 1.f, -1.f, 0,    1.f, 1.f, 1.f,  // bottomm right
			-1.f,  1.f, 0,    1.f, 1.f, 1.f,  // top left
			 1.f,  1.f, 0,    1.f, 1.f, 1.f   // top right
		};

	public:
		Background(const Vector3& color = {});

		~Background() {}

		void setColor(const Vector3& color) {
			_color = color;
			for (int i = 0; i < 4 * 6; i += 6) {
				QUAD_STRIP[i + 3] = color.x();
				QUAD_STRIP[i + 4] = color.y();
				QUAD_STRIP[i + 5] = color.z();
			}

			_vb.fill(QUAD_STRIP, sizeof(QUAD_STRIP));
			_vb.unbind();
		}

		const Vector3& getColor() const {
			return _color;
		}

		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override;

	};



}
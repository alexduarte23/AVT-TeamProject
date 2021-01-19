#pragma once

#include "SceneNode.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace avt {

	class Background : public SceneNode{
	private:

		VertexArray _va;
		VertexBuffer _vb;

		static const float QUAD_STRIP[4 * 6];

	public:
		Background(const Vector3& color = {});

		~Background() {}

		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override;

	};



}
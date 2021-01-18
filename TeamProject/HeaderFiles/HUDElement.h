#pragma once

#include "Texture.h"
#include "SceneNode.h"

namespace avt {

	class HUDElement : public SceneNode {

		Texture _sprite;

		VertexArray _va;
		VertexBuffer _vb;

		static const float QUAD_STRIP[4 * 9];

	public:
		HUDElement(const std::string& textureFilename);

		~HUDElement () {}


		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override;


	};


}
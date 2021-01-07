#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "avt_math.h"

#include "Mesh.h"

namespace avt {
	class Cube : public Mesh{

	public:
		Cube() {/*
			//FACE1
			int i = 0;
			Vector4 color = Vector4(1.f, 0, 0, 1.f);
			addVertex({1.f, 1.f, 1.f}, color);
			addVertex({1.f, 1.f, -1.f}, color);
			addVertex({1.f, -1.f, 1.f}, color);
			addVertex({1.f, -1.f, -1.f}, color);
			addFace(i + 2, i + 1, i);
			addFace(i + 1, i + 2, i + 3);

			//FACE2
			i += 4;
			color = Vector4(0.0f, 1.0f, 1.0f, 1.f);
			addVertex({ -1.f, 1.f, 1.f }, color);
			addVertex({-1.f, 1.f, -1.f }, color);
			addVertex({-1.f, -1.f, 1.f }, color);
			addVertex({-1.f, -1.f, -1.f }, color);
			addFace(i, i + 1, i + 2);
			addFace(i + 3, i + 2, i + 1);

			//FACE3
			i += 4;
			color = Vector4(0, 1.0f, 0, 1.f);
			addVertex({ 1.f, 1.f, 1.f }, color);
			addVertex({ 1.f, 1.f, -1.f }, color);
			addVertex({ -1.f, 1.f, 1.f }, color);
			addVertex({ -1.f, 1.f, -1.f }, color);
			addFace(i, i + 1, i + 2);
			addFace(i + 3, i + 2, i + 1);

			//FACE4
			i += 4;
			color = Vector4(1.f, 0, 1.f, 1.f);
			addVertex({ 1.f, -1.f, 1.f }, color);
			addVertex({ 1.f, -1.f, -1.f }, color);
			addVertex({ -1.f, -1.f, 1.f }, color);
			addVertex({ -1.f, -1.f, -1.f }, color);
			addFace(i + 2, i + 1, i);
			addFace(i + 1, i + 2, i + 3);

			//FACE5
			i += 4;
			color = Vector4(0, 0, 1.f, 1.f);
			addVertex({ 1.f, 1.f, 1.f }, color);
			addVertex({ 1.f, -1.f, 1.f }, color);
			addVertex({ -1.f, 1.f, 1.f }, color);
			addVertex({ -1.f, -1.f, 1.f }, color);
			addFace(i + 2, i + 1, i);
			addFace(i + 1, i + 2, i + 3);

			//FACE6
			i += 4;
			color = Vector4(1.f, 1.f, 0, 1.f);
			addVertex({ 1.f, 1.f, -1.f }, color);
			addVertex({ 1.f, -1.f, -1.f }, color);
			addVertex({ -1.f, 1.f, -1.f }, color);
			addVertex({ -1.f, -1.f, -1.f }, color);
			addFace(i, i + 1, i + 2);
			addFace(i + 3, i + 2, i + 1);
		*/}
	};
}

#version 330 core

uniform float Scale;
uniform mat3 Kernel;
uniform int Mode;

mat3 OffsetX = mat3(-1, -1, -1, 0, 0, 0, 1, 1, 1);
mat3 OffsetY = mat3(-1, 0, 1, -1, 0, 1, -1, 0, 1);

in vec2 exTexcoord;
out vec4 FragmentColor;

void main(void) 
{
	vec2 texcoord = vec2(exTexcoord.x, 1 - exTexcoord.y);
	switch (Mode) {
	case 0:
		FragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
		break;
	case 1:
		vec3 sum = vec3(0.0);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				vec2 offset = Scale * vec2(OffsetX[i][j], OffsetY[i][j]);
				vec2 new_texcoord = texcoord + offset;
				float k = Kernel[i][j];
				sum += k * vec3(1.0, 0.0, 0.0);
			}
		}
		FragmentColor = vec4(sum, 1.0);
		break;
	}
}

#version 330 core

layout(location=0) in vec3 in_vertex;
layout(location=1) in vec2 in_texCoord;

layout(location=2) in vec3 in_pos;
layout(location=3) in vec4 in_color;
layout(location=4) in float in_size;
layout(location=5) in float in_rot;

out vec4 ex_color;
out vec2 texCoord;

uniform mat4 ModelMatrix;

uniform SharedMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void) { // billboarding shader

	// add particle translation to apply billboarding
	mat4 translation = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, in_pos.x,in_pos.y,in_pos.z,1);
	mat4 modelView = ViewMatrix * ModelMatrix * translation;

	// remove rotation but keep scale and add particle size
	float scaleX = length(vec3(modelView[0][0], modelView[1][0], modelView[2][0]));
	float scaleY = length(vec3(modelView[0][1], modelView[1][1], modelView[2][1]));
	float scaleZ = length(vec3(modelView[0][2], modelView[1][2], modelView[2][2]));

	modelView[0][0] = scaleX * in_size; 
	modelView[0][1] = 0.0; 
	modelView[0][2] = 0.0; 
	
	modelView[1][0] = 0.0; 
	modelView[1][1] = scaleY * in_size; 
	modelView[1][2] = 0.0; 
	
	modelView[2][0] = 0.0; 
	modelView[2][1] = 0.0; 
	modelView[2][2] = scaleZ * in_size;

	// apply particle rotation after being billboarded
	mat4 rot = mat4(cos(in_rot),sin(in_rot),0,0, -sin(in_rot),cos(in_rot),0,0, 0,0,1,0, 0,0,0,1);
	modelView *= rot;

	gl_Position = ProjectionMatrix * modelView * vec4(in_vertex,1);
	
	ex_color = in_color;
	texCoord = in_texCoord;
}
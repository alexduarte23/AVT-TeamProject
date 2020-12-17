#version 330 core

layout(location=0) in vec3 in_vertex;
layout(location=1) in vec3 in_pos;
layout(location=2) in vec4 in_color;
layout(location=3) in float in_size;

out vec4 ex_color;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	mat4 transform = mat4(in_size,0,0,0, 0,in_size,0,0, 0,0,in_size,0, in_size*in_pos.x,in_size*in_pos.y,in_size*in_pos.z,1);
	mat4 modelView = ViewMatrix * ModelMatrix * transform;

	modelView[0][0] = 1.0; 
	modelView[0][1] = 0.0; 
	modelView[0][2] = 0.0; 
	
	modelView[1][1] = 1.0; 
	modelView[1][0] = 0.0; 
	modelView[1][2] = 0.0; 
	
	modelView[2][0] = 0.0; 
	modelView[2][1] = 0.0; 
	modelView[2][2] = 1.0;

	gl_Position = ProjectionMatrix * modelView * vec4(in_vertex,1);// * in_size + in_pos, 1);
	ex_color = in_color;
}
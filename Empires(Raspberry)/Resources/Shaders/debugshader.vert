#version 300 es
precision mediump float;
layout (location = 0) in vec3 a_position;

uniform mat4 MVP;

void main()               
{ 						
	gl_Position = MVP * vec4(a_position, 1);
}      
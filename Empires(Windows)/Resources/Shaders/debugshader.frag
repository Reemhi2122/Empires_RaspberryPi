#version 330 core
precision mediump float;     

out vec4 FragColor;
uniform vec4  Ambient;

void main()                                         
{                                                   
	FragColor = Ambient;
}
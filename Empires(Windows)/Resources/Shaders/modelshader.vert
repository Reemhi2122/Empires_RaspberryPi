#version 330 core
precision mediump float;
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;

out vec3 vertexColor;
out vec4 v_normal;
out vec2 v_texCoord;
out vec3 v_lightDir;

uniform mat4 MVP;
uniform vec3 faction_color;
uniform vec3 lightDir;

void main()
{
	gl_Position = MVP * vec4(a_position, 1);
    vertexColor = faction_color;
	v_texCoord = a_texCoord;
	v_normal = vec4(a_normal, 1);
	v_lightDir = lightDir;
}   
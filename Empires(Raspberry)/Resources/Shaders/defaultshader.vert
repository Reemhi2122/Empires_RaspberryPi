#version 300 es
precision highp float;
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;

out vec3 vertexColor;
out vec3 v_normal;
out vec2 v_texCoord;
out vec3 v_lightDir;
out vec3 v_fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 faction_color;
uniform vec3 lightDir;

void main()
{
	gl_Position = projection * view * model * vec4(a_position, 1);
    vertexColor = faction_color;
	v_texCoord = a_texCoord;
	vec4 rotatedNormal = (projection * view * model) * vec4(a_normal, 0);
	v_normal = vec3(rotatedNormal.x, rotatedNormal.y, rotatedNormal.z);
	v_lightDir = normalize(vec3(view * vec4(lightDir, 1.0)));
	v_fragPos = vec3(view * model * vec4(a_position, 1));
}
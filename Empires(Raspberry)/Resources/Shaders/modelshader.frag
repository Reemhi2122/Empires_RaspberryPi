#version 300 es
precision mediump float; 
out vec4 FragColor;

in vec3 vertexColor;
in vec4 v_normal;
in vec2 v_texCoord;
in vec3 v_lightDir;

uniform sampler2D s_texture;

void main()                                         
{
	FragColor = texture2D( s_texture, v_texCoord ) * vec4(vertexColor, 1);
}
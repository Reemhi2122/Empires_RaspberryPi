#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinate;

out vec3 vertexColor;
out vec2 TextureCoordinate;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    vertexColor = aColor;
    TextureCoordinate = aTextureCoordinate;
}

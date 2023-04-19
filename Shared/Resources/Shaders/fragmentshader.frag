#version 300 es
precision mediump float; 
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TextureCoordinate;

uniform sampler2D aTexture;

void main()
{
    FragColor = texture(aTexture, TextureCoordinate);
} 

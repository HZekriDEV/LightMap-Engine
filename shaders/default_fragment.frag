#version 330 core

out vec4 PixelColor;
  
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    PixelColor = vec4(vertexColor, 1.0f);
}
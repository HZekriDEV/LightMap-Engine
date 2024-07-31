#version 330 core

in vec4 vertexColor;
out vec4 screenColor;

void main()
{
	screenColor = vertexColor;
} 
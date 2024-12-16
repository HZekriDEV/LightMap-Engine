#version 330 core

out vec4 screenColor;
  
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    screenColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    //screenColor = vec4(vertexColor, 1.0f);

}
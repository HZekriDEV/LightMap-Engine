#version 330 core

out vec4 PixelColor;
  
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    PixelColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    //PixelColor = vec4(vertexColor, 1.0f);

}
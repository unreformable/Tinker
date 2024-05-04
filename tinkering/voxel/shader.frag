#version 450 core

in vec3 fCol;
in vec2 fTexCoord;

//uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
   FragColor = vec4(fCol, 1.0); //* texture(uTexture, fTexCoord)
}
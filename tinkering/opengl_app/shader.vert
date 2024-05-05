#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 fCol;
out vec2 fTexCoord;

void main()
{
   gl_Position = uProj * uView * uWorld * vec4(vPos, 1.0);
   fCol = vCol;
   fTexCoord = vTexCoord;
}
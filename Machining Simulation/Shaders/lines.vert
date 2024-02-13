#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec4 color;
uniform mat4 proj;

out vec4 LineColor;

void main()
{
	LineColor = color;

   gl_Position = proj * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
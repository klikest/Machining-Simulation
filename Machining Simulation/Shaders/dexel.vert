#version 330

layout (location = 0) in vec3 position;  
layout (location = 1) in vec4 aOffset;
layout (location = 2) in vec3 aColor;

out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;

void main(void) 
{

    vColor = vec4(aColor, 1.0f); 

    gl_Position = proj * view * vec4( position.x + aOffset.x, position.y + aOffset.y, aOffset.w * position.z + aOffset.z, 1.0);

}
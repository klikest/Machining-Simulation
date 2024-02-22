#version 330

layout (location = 0) in vec3 position;  
layout (location = 1) in vec4 aOffset;

out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;

void main(void) 
{

    vColor = vec4(0.5, 0.5, 0.5, 1.0f); 
   
    //gl_Position = proj * view * vec4( vec3( (position.x + aOffset.x), (position.y + aOffset.y), (position.z*aOffset.w + aOffset.z) ), 1.0);

    gl_Position = proj * view * vec4( position, 1.0);

}
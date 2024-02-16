#version 330

layout (location=0) in vec3 position;  
layout (location=1) in vec3 color;  

out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;

void main(void) 
{

    vColor = vec4(color, 1.0f); 
   
    gl_Position = proj * view * vec4(position, 1.0f);

}
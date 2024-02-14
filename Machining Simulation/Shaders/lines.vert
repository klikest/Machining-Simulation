#version 430

layout (location=0) in vec3 position;  // coord

out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;

void main(void) 
{

    vColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
   
    gl_Position = proj * view * vec4(position, 1.0f);

}
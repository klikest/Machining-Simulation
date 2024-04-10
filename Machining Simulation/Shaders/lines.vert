#version 330 core

layout (location=0) in vec3 position;  
layout (location=1) in vec3 color;  

out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;



uniform bool Machine_is_transform;

float new_x;
float new_y;
float new_z;




vec4 new_position;

void main(void) 
{

    vColor = vec4(color, 1.0f); 
   

    if(color != vec3(1, 1, 1))
    {
        if(Machine_is_transform)
        {
            new_position = vec4(position.x, position.y, position.z, 1.0f);
    
            new_x = new_position.x;
            new_y = new_position.y;
            new_z = new_position.z ;
        }
        else
        {
            new_x = position.x;
            new_y = position.y;
            new_z = position.z;
        }

    }

    else
    {
        new_x = position.x;
        new_y = position.y;
        new_z = position.z;
    }


    gl_Position = proj * view * vec4(new_x, new_y, new_z, 1.0f);

}




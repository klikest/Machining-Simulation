#version 330 core

layout (location=0) in vec3 position;  
layout (location=1) in vec3 color;  

out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;


uniform float Machine_X;
uniform float Machine_offset;
uniform float Machine_A;
uniform float Machine_C;
uniform bool Machine_is_transform;

float new_x;
float new_y;
float new_z;


mat4 rotationMatrix(vec3 axis, float angle);

mat4 rot_mat_c;

mat4 rot_mat_a;


vec4 new_position;

void main(void) 
{

    vColor = vec4(color, 1.0f); 
   

    if(color != vec3(1, 1, 1))
    {
        if(Machine_is_transform)
        {
            mat4 rot_mat_c = rotationMatrix( vec3(0, 0, 1), radians(Machine_C) );

            new_position = vec4(position.x, position.y, position.z, 1.0f);
    
            new_position = rot_mat_c * new_position;

            new_position = new_position + vec4(0, 0, -Machine_offset, 0);
        
            mat4 rot_mat_a = rotationMatrix( vec3(0, 1, 0), radians(Machine_A) );

            //new_position = rot_mat_a * new_position;

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



mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}
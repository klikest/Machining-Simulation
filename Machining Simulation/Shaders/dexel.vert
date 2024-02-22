#version 330

layout (location = 0) in vec3 cube_pos;  
layout (location = 1) in vec3 dexel_data;


out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;

uniform float x_size;
uniform float y_size;

void main(void) 
{
    float start = dexel_data.x;
    float end = dexel_data.y;
    float len = end - start;

    float new_x = cube_pos.x + mod(gl_InstanceID, x_size);
    float new_y = cube_pos.y + mod( (gl_InstanceID / x_size), y_size );
    float new_z = len * cube_pos.z + start;

    gl_Position = proj * view * vec4( new_x, new_y, new_z, 1.0);


    float color = dexel_data.z;

        if (color == 1)
    {
        vColor = vec4(1, 0, 0, 1.0f);
    }

        else if (color == 2)
    {
        vColor = vec4(0, 1, 0, 1.0f);
    }

        else if (color == 3)
    {
        vColor = vec4(0, 0, 1, 1.0f);
    }

        else
    {
        vColor = vec4(0.5, 0.5, 0.5, 1.0f);
    }





}
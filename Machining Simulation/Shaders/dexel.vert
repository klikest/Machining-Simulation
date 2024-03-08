#version 330

layout (location = 0) in vec3 cube_pos;  
layout (location = 1) in vec4 dexel_data;
layout (location = 3) in float in_color;


out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;

uniform vec3 colors_choose[3];
uniform vec3 cam_pos;

uniform float acc;

vec3 normal;
vec3 lightDir;
float diff;
vec3 get_normal(int i);

void main(void) 
{

    float new_x = (cube_pos.x + dexel_data.x) * acc;
    float new_y = (cube_pos.y + dexel_data.y) * acc;
    float new_z = (dexel_data.w * cube_pos.z + dexel_data.z) * acc;

    gl_Position = proj * view * vec4( new_x, new_y, new_z, 1.0) ;



    lightDir = cam_pos - vec3(new_x, new_y, new_z);
    normal = get_normal(gl_VertexID);
    diff = max(dot(normalize(normal), normalize(lightDir)), 0.0);



    int color_num = int(in_color);

    // defline color of dexel
    vec4 dexel_color = vec4( colors_choose[color_num], 1.0f);
    vColor = diff * dexel_color + vec4(0.1, 0.1, 0.1, 0) ;




}


vec3 get_normal(int i)
{
    
    if (i < 6)
    {
        normal = vec3(0.0, 0.0, -1.0);
    }

    else if (i >= 6 && i < 12)
    {
        normal = vec3(1.0, 0.0, 0.0);
    }

    else if (i >= 12 && i < 18)
    {
        normal = vec3(0.0, 0.0, 1.0);
    }

    else if (i >= 18 && i < 24)
    {
        normal = vec3(-1.0, 0.0, 0.0);
    }

    else if (i >= 24 && i < 30)
    {
        normal = vec3(0.0, -1.0, 0.0);
    }

        else if (i >= 30 && i < 36)
    {
        normal = vec3(0.0, 1.0, 0.0);
    }
    return normal;
}
#version 330

layout (location = 0) in vec3 cube_pos;  
layout (location = 1) in vec4 dexel_data;
layout (location = 3) in float in_color;
layout (location = 4) in vec3 in_normal;


out vec4 vColor;  

uniform mat4 view;
uniform mat4 proj;

uniform vec3 colors_choose[3];
uniform vec3 cam_pos;

uniform float acc;


uniform bool Machine_is_transform;


uniform mat4 inv_transform_mat;

vec3 normal;
vec3 lightDir;
float diff;



vec4 new_position;

vec4 new_normal;

void main(void) 
{



    float new_x = ((cube_pos.x + dexel_data.x) * acc)  ;
    float new_y = (cube_pos.y + dexel_data.y) * acc;
    float new_z = ((dexel_data.w * cube_pos.z + dexel_data.z))  ;

    if(Machine_is_transform)
    {
    
        new_position = inv_transform_mat * vec4(new_x, new_y, new_z, 1.0f);
    
        new_x = new_position.x;
        new_y = new_position.y;
        new_z = new_position.z;



    }
    else
    {

    }

    normal = in_normal;  
        lightDir = cam_pos - vec3(new_x, new_y, new_z);  


    if(dexel_data.w == 0)
    {
        gl_Position = vec4(0, 0, 0, 1.0);
    }
    else
    {
        gl_Position = proj * view * vec4( new_x, new_y, new_z, 1.0) ;
    }



    //lightDir = cam_pos - vec3(new_x, new_y, new_z);
    
    diff = max(dot(normalize(in_normal), normalize(lightDir)), 0.0);



    int color_num = int(in_color);

    // defline color of dexel
    vec4 dexel_color = vec4( colors_choose[color_num], 1.0f);
    vColor = diff * dexel_color + vec4(0.1, 0.1, 0.1, 0) ;




}



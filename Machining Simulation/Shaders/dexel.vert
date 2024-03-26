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


uniform float Machine_X;
uniform float Machine_offset;
uniform float Machine_A;
uniform float Machine_C;
uniform bool Machine_is_transform;

vec3 normal;
vec3 lightDir;
float diff;
vec3 get_normal(int i);




mat4 rotationMatrix(vec3 axis, float angle);
mat4 rot_mat_c;
mat4 rot_mat_a;
vec4 new_position;

vec4 new_normal;

void main(void) 
{

    float new_x = ((cube_pos.x + dexel_data.x) * acc)  ;
    float new_y = (cube_pos.y + dexel_data.y) * acc;
    float new_z = ((dexel_data.w * cube_pos.z + dexel_data.z))  ;

    if(Machine_is_transform)
    {
        mat4 rot_mat_c = rotationMatrix( vec3(0, 0, 1), radians(Machine_C) );

        new_position = vec4(new_x, new_y, new_z, 1.0f);
    
        new_position = rot_mat_c * new_position;

        new_position = new_position + vec4(0, 0, -Machine_offset, 0);
        
        mat4 rot_mat_a = rotationMatrix( vec3(0, 1, 0), radians(Machine_A) );

        new_position = rot_mat_a * new_position;

        new_x = new_position.x;
        new_y = new_position.y;
        new_z = new_position.z;

        normal = get_normal(gl_VertexID);
        new_normal = vec4(normal, 1);
        new_normal = rot_mat_c * new_normal;
        normal = vec3(new_normal.x, new_normal.y, new_normal.z);


    }
    else
    {
        normal = get_normal(gl_VertexID);
    }

    


    if(dexel_data.w == 0)
    {
        gl_Position = vec4(0, 0, 0, 1.0);
    }
    else
    {
        gl_Position = proj * view * vec4( new_x, new_y, new_z, 1.0) ;
    }



    lightDir = cam_pos - vec3(new_x, new_y, new_z);
    
    diff = max(dot(normalize(normal), normalize(lightDir)), 0.0);



    int color_num = int(in_color);

    // defline color of dexel
    vec4 dexel_color = vec4( colors_choose[color_num], 1.0f);
    vColor = diff * dexel_color + vec4(0.1, 0.1, 0.1, 0) ;




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
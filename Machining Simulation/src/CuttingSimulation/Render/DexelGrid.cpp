#include "DexelGrid.h"


DexelGrid::DexelGrid()
{

}

bool Scalar_cyl(float r, float x, float y)
{
    if ((x * x) + (y * y) <= (r * r))
    {
        return true;
    }
    else
    {
        return false;
    }
}



glm::vec3 DexelGrid::transform(glm::vec3 point)
{
    glm::vec4 pos_1 = glm::vec4(point.x, point.y, point.z + offset, 0);

    glm::mat4 rotate_y = glm::mat4(1.0f);
    rotate_y = glm::rotate(rotate_y, glm::radians(A), glm::vec3(0.0f, 1.0f, 0.0f)); // ������� �������� ������ ��� �

    glm::vec4 pos_234 = (rotate_y * pos_1) + glm::vec4(Z, Y, -offset + X, 0);

    glm::mat4 rotate_x = glm::mat4(1.0f);
    rotate_x = glm::rotate(rotate_x, glm::radians(C), glm::vec3(0.0f, 0.0f, 1.0f)); // ������� �������� ������ ��� X

    glm::vec4 pos_5 = rotate_x * pos_234;
    
    return glm::vec3(pos_5.x, pos_5.y, pos_5.z);
}

glm::vec3 DexelGrid::inv_transform(glm::vec3 point)
{
    glm::mat4 rotate_x = glm::mat4(1.0f);
    rotate_x = glm::rotate(rotate_x, glm::radians(-C), glm::vec3(0.0f, 0.0f, 1.0f)); // ������� �������� ������ ��� X

    glm::vec4 pos_5 = rotate_x * glm::vec4(point.x, point.y, point.z, 1);
    glm::vec4 pos_43 = pos_5 - glm::vec4(Z, Y, X - offset, 0);

    glm::mat4 rotate_y = glm::mat4(1.0f);
    rotate_y = glm::rotate(rotate_y, glm::radians(-A), glm::vec3(0.0f, 1.0f, 0.0f)); // ������� �������� ������ ��� �

    glm::vec4 pos21 = (rotate_y * pos_43) - glm::vec4(0, 0, offset, 0);

    return glm::vec3(pos21.x, pos21.y, pos21.z);
}


struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Plane {
    glm::vec3 normal;
    float distance;
};


void DexelGrid::GenerateToolGrid()
{
    tool_near_plane.push_back(glm::vec3(10, 10, 0));
    tool_near_plane.push_back(glm::vec3(10, 10, H));

}

void DexelGrid::GenerateToolLines()
{
    tool_lines.clear();

    tool_lines.push_back(0);
    tool_lines.push_back(0);
    tool_lines.push_back(-offset);

    tool_lines.push_back(0);
    tool_lines.push_back(0);
    tool_lines.push_back(H);


    float pi = 2 * asin(1.0);

    for (float angle = 0; angle < 2*pi; angle += pi / 20)
    {
        tool_lines.push_back(sinf(angle)*D);
        tool_lines.push_back(cosf(angle)*D);
        tool_lines.push_back(0);

        tool_lines.push_back(sinf(angle) * D);
        tool_lines.push_back(cosf(angle) * D);
        tool_lines.push_back(H);
        /////////////////
        tool_lines.push_back(sinf(angle + pi / 20) * D);
        tool_lines.push_back(cosf(angle + pi / 20) * D);
        tool_lines.push_back(H);

        tool_lines.push_back(sinf(angle) * D);
        tool_lines.push_back(cosf(angle) * D);
        tool_lines.push_back(H);
        //////////////////
        tool_lines.push_back(sinf(angle + pi / 20) * D);
        tool_lines.push_back(cosf(angle + pi / 20) * D);
        tool_lines.push_back(0);

        tool_lines.push_back(sinf(angle) * D);
        tool_lines.push_back(cosf(angle) * D);
        tool_lines.push_back(0);
    }


    for (int i = 0; i < tool_lines.size(); i+=3)
    {
        glm::vec3 new_pos = transform(glm::vec3(tool_lines[i], tool_lines[i+1], tool_lines[i+2]));

        tool_lines[i] = new_pos.x;
        tool_lines[i+1] = new_pos.y;
        tool_lines[i+2] = new_pos.z;
    }

}


void DexelGrid::CreateBlankCyl(float diam, float h, float acc_)
{
    acc = acc_;

    if ( (int)ceil(diam / acc)%2 == 1)
    {
        X_size = ceil(diam / acc) ;
        Y_size = ceil(diam / acc) ;
    }
    else
    {
        X_size = ceil(diam / acc) + 1;
        Y_size = ceil(diam / acc) + 1;
    }

    rect_min = glm::vec3(acc * -X_size / 2, acc * -Y_size / 2, 0);
    rect_max = glm::vec3(acc * X_size / 2, acc * Y_size / 2, h);


    num_dexels = new int[X_size * Y_size];
    grid_list = new Dexel* [X_size * Y_size];
    summ_num_of_dexels = 0;

    for (int i = 0; i < X_size * Y_size; i++)
    {
        float x = acc * ( (i % X_size) - (X_size / 2) );
        float y = acc * ( ((i / X_size) % Y_size) - (Y_size / 2) );


        grid_list[i] = new Dexel[1];
        num_dexels[i] = 1;

        if (Scalar_cyl(diam / 2, x, y))
        {

            grid_list[i][0].start = 0;
            grid_list[i][0].end = h;
            grid_list[i][0].color = 0;

            summ_num_of_dexels += 1;
        }
        else
        {
            grid_list[i][0].start = 0;
            grid_list[i][0].end = 0;
            grid_list[i][0].color = 0;

        }

    }

}


void DexelGrid::GenerateDrawArrays()
{
    dexel_draw_data = new glm::vec4[summ_num_of_dexels];
    colors_dexels = new float[summ_num_of_dexels];

    int count = 0;

    for (int i = 0; i < X_size * Y_size; i++)
    {
        for (int num = 0; num < num_dexels[i]; num++)
        {
            float x = (i % X_size) - X_size / 2;
            float y = (i / X_size) % Y_size - Y_size / 2;
            float z = grid_list[i][num].start;
            float len = grid_list[i][num].end - grid_list[i][num].start;

            if (len != 0)
            {
                dexel_draw_data[count] = glm::vec4(x - acc/2, y - acc/2, z, len);
                colors_dexels[count] = grid_list[i][num].color;
                count += 1;
            }
        }


    }
}


void DexelGrid::DeleteArrays()
{
    for (int i = 0; i < X_size * Y_size; i++)
    {
        delete[] grid_list[i];
        grid_list[i] = nullptr;
    }

    delete[] grid_list;
    grid_list = nullptr;

    delete[] dexel_draw_data;
    dexel_draw_data = nullptr;

    delete[] colors_dexels;
    colors_dexels = nullptr;

}



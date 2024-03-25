#include "DexelGrid.h"

DexelGrid::DexelGrid()
{
    blank = new Blank();
    tool = new Tool();

    machine_coords.X = 80;
    machine_coords.A = -30;
    tool_D = 125;
    blank_D = 8;
    offset = 5;

    resolution = 0.3;

    TimeData.CreateArrays();
}




void DexelGrid::transformArray(std::vector<glm::vec3>& my_array)
{
    for (int i = 0; i < my_array.size(); i++)
    {
        my_array[i] = transform(my_array[i]);
    }
}

void DexelGrid::inv_transformArray(std::vector<glm::vec3>& my_array)
{
    for (int i = 0; i < my_array.size(); i++)
    {
        my_array[i] = inv_transform(my_array[i]);
    }
}

glm::vec3 DexelGrid::transform(glm::vec3 point)
{
    glm::vec4 pos_1 = glm::vec4(point.x, point.y, point.z + offset, 0);

    glm::mat4 rotate_y = glm::mat4(1.0f);
    rotate_y = glm::rotate(rotate_y, glm::radians(A), glm::vec3(0.0f, 1.0f, 0.0f)); // Матрица поворота вокруг оси А

    glm::vec4 pos_234 = (rotate_y * pos_1) + glm::vec4(Z, Y, -offset + X, 0);

    glm::mat4 rotate_x = glm::mat4(1.0f);
    rotate_x = glm::rotate(rotate_x, glm::radians(C), glm::vec3(0.0f, 0.0f, 1.0f)); // Матрица поворота вокруг оси X

    glm::vec4 pos_5 = rotate_x * pos_234;
    
    return glm::vec3(pos_5.x, pos_5.y, pos_5.z);
}

glm::vec3 DexelGrid::inv_transform(glm::vec3 point)
{

    glm::mat4 rotate_x = glm::mat4(1.0f);
    rotate_x = glm::rotate(rotate_x, glm::radians(-C), glm::vec3(0.0f, 0.0f, 1.0f)); // Матрица поворота вокруг оси X

    glm::vec4 pos_5 = rotate_x * glm::vec4(point.x, point.y, point.z, 1);
    glm::vec4 pos_43 = pos_5 - glm::vec4(Z, Y, X - offset, 0);

    glm::mat4 rotate_y = glm::mat4(1.0f);
    rotate_y = glm::rotate(rotate_y, glm::radians(-A), glm::vec3(0.0f, 1.0f, 0.0f)); // Матрица поворота вокруг оси А

    glm::vec4 pos21 = (rotate_y * pos_43) - glm::vec4(0, 0, offset, 0);

    return glm::vec3(pos21.x, pos21.y, pos21.z);
    
}





void DexelGrid::Generate_Draw_Arrays(Blank* blank, Tool* tool)
{    
    if (blank->Grid != nullptr)
    {
        DeleteDrawArrays();

        int array_size = blank->Num_of_Dexels + tool->Num_of_Dexels;

        dexel_draw_data = new glm::vec4[array_size];
        colors_dexels = new float[array_size];

        int count = 0;

        for (int i = 0; i < blank->Grid_size; i++)
        {
            for (int j = 0; j < blank->Grid[i].size(); j++)
            {
                if (blank->Grid[i][j].color > -1)
                {
                    dexel_draw_data[count] = blank->Get_Dexel_To_Draw(i, j);
                    colors_dexels[count] = blank->Grid[i][j].color;

                    count += 1;
                }

            }

            if (tool->Grid != nullptr && tool->Grid[i].color > -1)
            {
                dexel_draw_data[count] = tool->Get_Dexel_To_Draw(i);
                colors_dexels[count] = tool->Grid[i].color;

                count += 1;
            }
        }
    }
}





void DexelGrid::BooleanOperation(Blank* blank, Tool* tool)
{
    for (int i = 0; i < blank->Grid_size; i++)
    {
        float tool_start = tool->Grid[i].start;
        float tool_end = tool->Grid[i].end;

        for (int num = 0; num < blank->Grid[i].size(); num++)
        {
            float blank_start = blank->Grid[i][num].start;
            float blank_end = blank->Grid[i][num].end ;

            if (blank->Grid[i][num].color > -1 && tool->Grid[i].color > -1)
            {
                //         Tool
                //    --------------
                //           ---------------
                //                Blank
                if (tool_end >= blank_start &&
                    tool_start <= blank_start &&
                    tool_end <= blank_end)
                {
                    blank->Grid[i][num].start = tool_end;
                }

                //                  Tool
                //             --------------
                //    ---------------
                //         Blank
                if (tool_start > blank_start &&
                    tool_start < blank_end &&
                    tool_end > blank_end)
                {
                    blank->Grid[i][num].end = tool_start;
                }

                //             Tool
                //     --------------------
                //         ------------
                //             Blank
                if (tool_start < blank_start &&
                    tool_end > blank_end)
                {

                    blank->Grid[i].erase(blank->Grid[i].begin() + num);
                    blank->Num_of_Dexels -= 1;
                }


                //             Tool
                //         ------------
                //     ---------------------
                //             Blank
                if (tool_start > blank_start &&
                    tool_end < blank_end)
                {
                    blank->Grid[i][num].end = tool_start;
                    blank->Grid[i].push_back(Dexel(tool_end, blank_end, blank->Grid[i][num].color));
                    blank->Num_of_Dexels += 1;
                }
                
            }
        }

    }
}



void DexelGrid::DeleteDrawArrays()
{
    if (dexel_draw_data != nullptr)
    {
        delete[] dexel_draw_data;
        dexel_draw_data = nullptr;
    }

    if (colors_dexels != nullptr)
    {
        delete[] colors_dexels;
        colors_dexels = nullptr;
    }

}




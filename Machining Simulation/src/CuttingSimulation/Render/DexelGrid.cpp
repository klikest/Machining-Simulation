#include "DexelGrid.h"

DexelGrid::DexelGrid()
{
    blank = new Blank();
    tool = new Tool();

    machine_coords.X = -130;
    machine_coords.Y = 68;
    machine_coords.Z = 0;
    machine_coords.A = 30;
    tool_D = 125;
    tool_H = 10;
    blank_D = 20;
    blank_H = 100;
    machine_coords.offset = 15;

    resolution = 1;

    TimeData.CreateArrays();
}



void DexelGrid::Generate_Draw_Arrays(Blank* blank, Tool* tool)
{    
    if (blank->Grid != nullptr)
    {
        DeleteDrawArrays();

        int array_size = blank->Num_of_Dexels + tool->Num_of_Dexels;

        dexel_draw_data = new glm::vec4[array_size];
        colors_dexels = new float[array_size];
        normals_dexels = new glm::vec3[array_size];
        int count = 0;

        for (int i = 0; i < blank->Grid_size; i++)
        {
            for (int j = 0; j < blank->Grid[i].size(); j++)
            {
                if (blank->Grid[i][j].color > -1)
                {
                    dexel_draw_data[count] = blank->Get_Dexel_To_Draw(i, j);
                    colors_dexels[count] = blank->Grid[i][j].color;
                    normals_dexels[count] = blank->Grid[i][j].normal;

                    count += 1;
                }

            }

            if (tool->Grid != nullptr && tool->Grid[i].color > -1)
            {
                dexel_draw_data[count] = tool->Get_Dexel_To_Draw(i);
                colors_dexels[count] = tool->Grid[i].color;
                normals_dexels[count] = glm::vec3(1,1,1);

                count += 1;
            }
        }
    }
}





void DexelGrid::BooleanOperation(Blank* blank, Tool* tool)
{
    len_of_cut = 0;

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
                    len_of_cut += tool_end - blank_start;
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
                    len_of_cut += blank_end - tool_start;
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
                    len_of_cut += blank_end - blank_start;
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
                    len_of_cut += tool_end - tool_start;
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

    if (normals_dexels != nullptr)
    {
        delete[] normals_dexels;
        normals_dexels = nullptr;
    }

}




#include "DexelGrid.h"

DexelGrid::DexelGrid()
{
    blank = new Blank();
    tool = new Tool();

    machine_coords.X = -80;
    machine_coords.A = -30;
    tool_D = 40;
    tool_H = 10;
    blank_D = 20;
    machine_coords.offset = 15;

    resolution = 0.2;

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




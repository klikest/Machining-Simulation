#include "DexelGrid.h"

DexelGrid::DexelGrid()
{
    blank = new Blank();
    tool = new Tool();

    machine_coords.X = -130;
    machine_coords.Y = 68;
    machine_coords.Z = 0;
    machine_coords.A = 15;
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

            for (int j = 0; j < tool->Grid[i].size(); j++)
            {
                if (tool->Grid != nullptr && tool->Grid[i][j].color > -1)
                {
                    dexel_draw_data[count] = tool->Get_Dexel_To_Draw(i, j);
                    colors_dexels[count] = tool->Grid[i][j].color;
                    normals_dexels[count] = tool->Grid[i][j].normal;

                    count += 1;
                }

            }

        }
    }
}


void DexelGrid::Set_Normals_and_Color(Blank* blank, Tool* tool)
{

    float acc = blank->resolution / 10;

    for (int i = 0; i < blank->Grid_size; i++)
    {
        if (i > blank->X_grid_size && i < blank->Grid_size - blank->X_grid_size &&  tool->Grid[i][0].color > -1) /// под декселем инструмента находится дексель заготовки
        {
            for (int tool_dexel = 0; tool_dexel < tool->Grid[i].size(); tool_dexel++)
            {
                float tool_start = tool->Grid[i][tool_dexel].start;
                float tool_end = tool->Grid[i][tool_dexel].end;
                int tool_color = tool->Grid[i][tool_dexel].color;
                glm::vec3 tool_normal = tool->Grid[i][tool_dexel].normal;

                for (int num = 0; num < blank->Grid[i - blank->X_grid_size].size(); num++)
                {
                    float blank_start = blank->Grid[i - blank->X_grid_size][num].start;
                    float blank_end = blank->Grid[i - blank->X_grid_size][num].end;
                    int blank_color = blank->Grid[i - blank->X_grid_size][num].color;
                    glm::vec3 blank_normal = blank->Grid[i - blank->X_grid_size][num].normal;

                    if (blank->Grid[i - blank->X_grid_size][num].color > -1 && tool->Grid[i][tool_dexel].color > -1 && blank_color != tool_color)
                    {
                        //         Tool
                        //    --------------
                        //           ---------------
                        //                Blank
                        if (tool_end > blank_start &&
                            tool_start < blank_start &&
                            tool_end < blank_end)
                        {   
                            blank->Grid[i - blank->X_grid_size][num].start = tool_end;
                            blank->Grid[i - blank->X_grid_size].push_back(Dexel(blank_start, tool_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));
                            
                            len_of_cut += tool_end - blank_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //                  Tool
                        //             --------------
                        //    ---------------
                        //         Blank
                        if (tool_start > blank_start &&
                            tool_start < blank_end &&
                            tool_end > blank_end)
                        {
                            blank->Grid[i - blank->X_grid_size].push_back(Dexel(tool_start, blank_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));
                            blank->Grid[i - blank->X_grid_size][num].end = tool_start;
                            len_of_cut += blank_end - tool_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //             Tool
                        //     --------------------
                        //         ------------
                        //             Blank
                        if (tool_start < blank_start &&
                            tool_end > blank_end)
                        {

                            blank->Grid[i - blank->X_grid_size][num].normal = -tool->Grid[i][tool_dexel].normal;
                            blank->Grid[i - blank->X_grid_size][num].color = tool->Grid[i][tool_dexel].color;
                        }


                        //             Tool
                        //         ------------
                        //     ---------------------
                        //             Blank
                        if (tool_start > blank_start &&
                            tool_end < blank_end)
                        {
                            Dexel new_left_dexel = Dexel(blank_start, tool_start, blank_color, blank_normal);
                            Dexel new_right_dexel = Dexel(tool_end, blank_end, blank_color, blank_normal);
                            Dexel new_middle_dexel = Dexel(tool_start, tool_end, tool_color, -tool_normal);
                            

                            blank->Grid[i - blank->X_grid_size][num] = new_left_dexel;
                            blank->Grid[i - blank->X_grid_size].push_back(new_middle_dexel);
                            blank->Grid[i - blank->X_grid_size].push_back(new_right_dexel);
                            
                            blank->Num_of_Dexels += 2;
                        }

                    }
                    
                }


                for (int num = 0; num < blank->Grid[i + blank->X_grid_size].size(); num++)
                {
                    float blank_start = blank->Grid[i + blank->X_grid_size][num].start;
                    float blank_end = blank->Grid[i + blank->X_grid_size][num].end;
                    int blank_color = blank->Grid[i + blank->X_grid_size][num].color;
                    glm::vec3 blank_normal = blank->Grid[i + blank->X_grid_size][num].normal;

                    if (blank->Grid[i + blank->X_grid_size][num].color > -1 && tool->Grid[i][tool_dexel].color > -1 && blank_color != tool_color)
                    {
                        //         Tool
                        //    --------------
                        //           ---------------
                        //                Blank
                        if (tool_end > blank_start &&
                            tool_start < blank_start &&
                            tool_end < blank_end)
                        {
                            blank->Grid[i + blank->X_grid_size][num].start = tool_end;
                            blank->Grid[i + blank->X_grid_size].push_back(Dexel(blank_start, tool_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));

                            len_of_cut += tool_end - blank_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //                  Tool
                        //             --------------
                        //    ---------------
                        //         Blank
                        if (tool_start > blank_start &&
                            tool_start < blank_end &&
                            tool_end > blank_end)
                        {
                            blank->Grid[i + blank->X_grid_size].push_back(Dexel(tool_start, blank_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));
                            blank->Grid[i + blank->X_grid_size][num].end = tool_start;
                            len_of_cut += blank_end - tool_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //             Tool
                        //     --------------------
                        //         ------------
                        //             Blank
                        if (tool_start < blank_start &&
                            tool_end > blank_end)
                        {

                            blank->Grid[i + blank->X_grid_size][num].normal = -tool->Grid[i][tool_dexel].normal;
                            blank->Grid[i + blank->X_grid_size][num].color = tool->Grid[i][tool_dexel].color;
                        }


                        //             Tool
                        //         ------------
                        //     ---------------------
                        //             Blank
                        if (tool_start > blank_start &&
                            tool_end < blank_end)
                        {
                            Dexel new_left_dexel = Dexel(blank_start, tool_start, blank_color, blank_normal);
                            Dexel new_right_dexel = Dexel(tool_end, blank_end, blank_color, blank_normal);
                            Dexel new_middle_dexel = Dexel(tool_start, tool_end, tool_color, -tool_normal);


                            blank->Grid[i + blank->X_grid_size][num] = new_left_dexel;
                            blank->Grid[i + blank->X_grid_size].push_back(new_middle_dexel);
                            blank->Grid[i + blank->X_grid_size].push_back(new_right_dexel);

                            blank->Num_of_Dexels += 2;
                        }

                    }

                }


            }
        }


        if (i > 0 && i < blank->Grid_size - 1 && tool->Grid[i][0].color > -1) /// справа или слева от декселя инструмента находится дексель заготовки
        {
            for (int tool_dexel = 0; tool_dexel < tool->Grid[i].size(); tool_dexel++)
            {
                float tool_start = tool->Grid[i][tool_dexel].start;
                float tool_end = tool->Grid[i][tool_dexel].end;
                int tool_color = tool->Grid[i][tool_dexel].color;
                glm::vec3 tool_normal = tool->Grid[i][tool_dexel].normal;

                for (int num = 0; num < blank->Grid[i - 1].size(); num++)
                {
                    float blank_start = blank->Grid[i - 1][num].start;
                    float blank_end = blank->Grid[i - 1][num].end;
                    int blank_color = blank->Grid[i - 1][num].color;
                    glm::vec3 blank_normal = blank->Grid[i - 1][num].normal;

                    if (blank->Grid[i - 1][num].color > -1 && tool->Grid[i][tool_dexel].color > -1)
                    {
                        //         Tool
                        //    --------------
                        //           ---------------
                        //                Blank
                        if (tool_end > blank_start &&
                            tool_start < blank_start &&
                            tool_end < blank_end)
                        {
                            blank->Grid[i - 1][num].start = tool_end;
                            blank->Grid[i - 1].push_back(Dexel(blank_start, tool_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));

                            len_of_cut += tool_end - blank_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //                  Tool
                        //             --------------
                        //    ---------------
                        //         Blank
                        if (tool_start > blank_start &&
                            tool_start < blank_end &&
                            tool_end > blank_end)
                        {
                            blank->Grid[i - 1].push_back(Dexel(tool_start, blank_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));
                            blank->Grid[i - 1][num].end = tool_start;
                            len_of_cut += blank_end - tool_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //             Tool
                        //     --------------------
                        //         ------------
                        //             Blank
                        if (tool_start < blank_start &&
                            tool_end > blank_end)
                        {

                            blank->Grid[i - 1][num].normal = -tool->Grid[i][tool_dexel].normal;
                            blank->Grid[i - 1][num].color = tool->Grid[i][tool_dexel].color;
                        }


                        //             Tool
                        //         ------------
                        //     ---------------------
                        //             Blank
                        if (tool_start > blank_start &&
                            tool_end < blank_end)
                        {
                            Dexel new_left_dexel = Dexel(blank_start, tool_start, blank_color, blank_normal);
                            Dexel new_right_dexel = Dexel(tool_end, blank_end, blank_color, blank_normal);
                            Dexel new_middle_dexel = Dexel(tool_start, tool_end, tool_color, -tool_normal);


                            blank->Grid[i - 1][num] = new_left_dexel;
                            blank->Grid[i - 1].push_back(new_middle_dexel);
                            blank->Grid[i - 1].push_back(new_right_dexel);

                            blank->Num_of_Dexels += 2;
                        }

                    }

                }


                for (int num = 0; num < blank->Grid[i + 1].size(); num++)
                {
                    float blank_start = blank->Grid[i + 1][num].start;
                    float blank_end = blank->Grid[i + 1][num].end;
                    int blank_color = blank->Grid[i + 1][num].color;
                    glm::vec3 blank_normal = blank->Grid[i + 1][num].normal;

                    if (blank->Grid[i + 1][num].color > -1 && tool->Grid[i][tool_dexel].color > -1)
                    {
                        //         Tool
                        //    --------------
                        //           ---------------
                        //                Blank
                        if (tool_end > blank_start &&
                            tool_start < blank_start &&
                            tool_end < blank_end)
                        {
                            blank->Grid[i + 1][num].start = tool_end;
                            blank->Grid[i + 1].push_back(Dexel(blank_start, tool_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));

                            len_of_cut += tool_end - blank_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //                  Tool
                        //             --------------
                        //    ---------------
                        //         Blank
                        if (tool_start > blank_start &&
                            tool_start < blank_end &&
                            tool_end > blank_end)
                        {
                            blank->Grid[i + 1].push_back(Dexel(tool_start, blank_end, tool->Grid[i][tool_dexel].color, -tool->Grid[i][tool_dexel].normal));
                            blank->Grid[i +1][num].end = tool_start;
                            len_of_cut += blank_end - tool_start;
                            blank->Num_of_Dexels += 1;
                        }

                        //             Tool
                        //     --------------------
                        //         ------------
                        //             Blank
                        if (tool_start < blank_start &&
                            tool_end > blank_end)
                        {

                            blank->Grid[i + 1][num].normal = -tool->Grid[i][tool_dexel].normal;
                            blank->Grid[i + 1][num].color = tool->Grid[i][tool_dexel].color;
                        }


                        //             Tool
                        //         ------------
                        //     ---------------------
                        //             Blank
                        if (tool_start > blank_start &&
                            tool_end < blank_end)
                        {
                            Dexel new_left_dexel = Dexel(blank_start, tool_start, blank_color, blank_normal);
                            Dexel new_right_dexel = Dexel(tool_end, blank_end, blank_color, blank_normal);
                            Dexel new_middle_dexel = Dexel(tool_start, tool_end, tool_color, -tool_normal);


                            blank->Grid[i + 1][num] = new_left_dexel;
                            blank->Grid[i + 1].push_back(new_middle_dexel);
                            blank->Grid[i + 1].push_back(new_right_dexel);

                            blank->Num_of_Dexels += 2;
                        }

                    }

                }


            }
        }





    }
}


void DexelGrid::BooleanOperation(Blank* blank, Tool* tool)
{
    len_of_cut = 0;

    for (int i = 0; i < blank->Grid_size; i++)
    { 

        for (int tool_dexel = 0; tool_dexel < tool->Grid[i].size(); tool_dexel++)
        {
            float tool_start = tool->Grid[i][tool_dexel].start;
            float tool_end = tool->Grid[i][tool_dexel].end;

            for (int num = 0; num < blank->Grid[i].size(); num++)
            {
                float blank_start = blank->Grid[i][num].start;
                float blank_end = blank->Grid[i][num].end;

                if (blank->Grid[i][num].color > -1 && tool->Grid[i][tool_dexel].color > -1)
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
                        blank->Grid[i].push_back(Dexel(tool_end, blank_end, blank->Grid[i][num].color, blank->Grid[i][num].normal));
                        blank->Num_of_Dexels += 1;
                        len_of_cut += tool_end - tool_start;
                    }

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




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

    resolution = 0.2;

    TimeData.CreateArrays();
}



void DexelGrid::Generate_Draw_Arrays(Blank* blank, Tool* tool)
{    

    int num_tool_dexels_in_list = 2;

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

            for (int j = 0; j < num_tool_dexels_in_list; j++)
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
    int num_tool_dexels_in_list = 2;
    float acc = blank->resolution / 10;

    /*

    for (int i = 0; i < blank->Grid_size; i++)
    {

        if (i > blank->X_grid_size && i < blank->Grid_size - blank->X_grid_size && tool->Grid[i][0].color > -1 && blank->Grid[i][0].color > -1)
        {
            for (int num = 0; num < blank->Grid[i].size(); num++)
            {

                float tool_up_min = tool->Grid[i + tool->X_grid_size][0].start;
                float tool_up_max = tool->Grid[i + tool->X_grid_size][0].end;

                float tool_down_min = tool->Grid[i - tool->X_grid_size][0].start;
                float tool_down_max = tool->Grid[i - tool->X_grid_size][0].end;

                float tool_left_min = tool->Grid[i + 1][0].start;
                float tool_left_max = tool->Grid[i + 1][0].end;

                float tool_right_min = tool->Grid[i - 1][0].start;
                float tool_right_max = tool->Grid[i - 1][0].end;


                float tool_start = fminf(fminf(tool_up_min, tool_down_min), fminf(tool_left_min, tool_right_min));
                float tool_end = fmaxf(fmaxf(tool_up_max, tool_down_max), fmaxf(tool_left_max, tool_right_max));

                glm::vec3 new_normal =
                    tool->Grid[i + tool->X_grid_size][0].normal
                    + tool->Grid[i - tool->X_grid_size][0].normal
                    + tool->Grid[i + 1][0].normal
                    + tool->Grid[i - 1][0].normal;


                float blank_start = blank->Grid[i][num].start;
                float blank_end = blank->Grid[i][num].end;
                int blank_color = blank->Grid[i][num].color;
                glm::vec3 blank_normal = blank->Grid[i][num].normal;



                if (true)
                {
                //         Tool
                //    --------------
                //           ---------------
                //                Blank
                    if (tool_end > blank_start &&
                        tool_start < blank_start &&
                        tool_end < blank_end)
                    {
                        blank->Grid[i][num].start = tool_end;
                        blank->Grid[i].push_back(Dexel(blank_start, tool_end, tool->Grid[i][0].color, -new_normal));

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
                        blank->Grid[i].push_back(Dexel(tool_start, blank_end, tool->Grid[i][0].color, -new_normal));
                        blank->Grid[i][num].end = tool_start;
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

                        blank->Grid[i][num].normal = -new_normal;
                        blank->Grid[i][num].color = tool->Grid[i][0].color;
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
                        Dexel new_middle_dexel = Dexel(tool_start, tool_end, tool->Grid[i][0].color, -new_normal);


                        blank->Grid[i][num] = new_left_dexel;
                        blank->Grid[i].push_back(new_middle_dexel);
                        blank->Grid[i].push_back(new_right_dexel);

                        blank->Num_of_Dexels += 2;
                    }
                }

               
            }
        }

    }
    */


    
    for (int i = 0; i < blank->Grid_size; i++)
    {
        if (i > blank->X_grid_size && i < blank->Grid_size - blank->X_grid_size &&  tool->Grid[i][0].color > -1) /// под декселем инструмента находится дексель заготовки
        {
            for (int tool_dexel = 0; tool_dexel < num_tool_dexels_in_list; tool_dexel++)
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
            for (int tool_dexel = 0; tool_dexel < num_tool_dexels_in_list; tool_dexel++)
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
                    
                    if (blank->Grid[i - 1][num].color > -1 && tool->Grid[i][tool_dexel].color > -1 && blank_color != tool_color)
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

                    if (blank->Grid[i + 1][num].color > -1 && tool->Grid[i][tool_dexel].color > -1 && blank_color != tool_color)
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
    int num_tool_dexels_in_list = 2;

    for (int i = 0; i < blank->Grid_size; i++)
    { 

        for (int tool_dexel = 0; tool_dexel < num_tool_dexels_in_list; tool_dexel++)
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

void DexelGrid::SimplifyBlank(Blank* blank)
{
    for (int i = 0; i < blank->Grid_size; i++)
    {


        for (int num = 0; num < blank->Grid[i].size() - 1; num++)
        {
            float blank_start_1 = blank->Grid[i][num].start;
            float blank_end_1 = blank->Grid[i][num].end;

            float blank_start_2 = blank->Grid[i][num + 1].start;
            float blank_end_2 = blank->Grid[i][num + 1].end;

            //                Blank dexel 1
            //              --------
            //           ---------------
            //                Blank dexel 2
            if (blank_start_1 >= blank_start_2 && blank_end_1 <= blank_end_2)
            {
                blank->Grid[i].erase(blank->Grid[i].begin() + num);
                blank->Num_of_Dexels -= 1;
            }


            //                Blank dexel 1
            //             -----------------
            //                -----------
            //                Blank dexel 2
            else if (blank_start_1 <= blank_start_2 && blank_end_1 >= blank_end_2)
            {
                blank->Grid[i].erase(blank->Grid[i].begin() + num + 1);
                blank->Num_of_Dexels -= 1;
            }


            //        С этим разобратся!

            //                Blank dexel 1
            //             --------
            //                     ------
            //                Blank dexel 2
            else if (fabsf(blank_end_1 - blank_start_2) < 0.01 && blank->Grid[i][num].color == blank->Grid[i][num + 1].color)
            {
                glm::vec3 delta_normal = blank->Grid[i][num].normal - blank->Grid[i][num+1].normal;
                
                if (delta_normal.x < blank->Grid[i][num].normal.x / 5 &&
                    delta_normal.y < blank->Grid[i][num].normal.y / 5 &&
                    delta_normal.z < blank->Grid[i][num].normal.z / 5)
                {
                    //blank->Grid[i][num].end = blank_end_2;
                    //blank->Grid[i].erase(blank->Grid[i].begin() + num + 1);
                    //blank->Num_of_Dexels -= 1;
                }
                
            }

            //                Blank dexel 1
            //                    --------
            //              ------
            //                Blank dexel 2
            else if (fabsf(blank_end_2 - blank_start_1) < 0.01 && blank->Grid[i][num].color == blank->Grid[i][num + 1].color)
            {
                glm::vec3 delta_normal = blank->Grid[i][num].normal - blank->Grid[i][num + 1].normal;

                if (delta_normal.x < blank->Grid[i][num].normal.x / 5 &&
                    delta_normal.y < blank->Grid[i][num].normal.y / 5 &&
                    delta_normal.z < blank->Grid[i][num].normal.z / 5)
                {
                    //blank->Grid[i][num].start = blank_start_2;
                    //blank->Grid[i].erase(blank->Grid[i].begin() + num + 1);
                    //blank->Num_of_Dexels -= 1;
                }

            }

            else if (fabsf(blank_end_1 - blank_start_1) < 0.001)
            {
                    blank->Grid[i].erase(blank->Grid[i].begin() + num);
                    blank->Num_of_Dexels -= 1;
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




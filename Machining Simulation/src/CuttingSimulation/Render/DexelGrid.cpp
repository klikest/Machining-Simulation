#include "DexelGrid.h"

DexelGrid::DexelGrid()
{

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



glm::vec4 DexelGrid::GetToolDexel(float dexel_x, float dexel_y)
{
    glm::vec3 origin_0 = glm::vec3(dexel_x, dexel_y, 0.0f);
    glm::vec3 origin_1 = glm::vec3(dexel_x, dexel_y, 2 * H);

    glm::vec3 inv_origin_0 = inv_transform(origin_0);
    glm::vec3 inv_origin_1 = inv_transform(origin_1);

    glm::vec3 inv_origin_dir = inv_origin_1 - inv_origin_0;



    Ray ray = { inv_origin_0, glm::normalize(inv_origin_dir) };

    Plane near_plane = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
    Plane far_plane = { glm::vec3(0.0f, 0.0f, H), glm::vec3(0.0f, 0.0f, 1.0f) };

    float denominator = glm::dot(near_plane.normal, ray.direction); // Равен нулю если вектор параллелен плоскости

    glm::vec3 intersectionPoint1, intersectionPoint2;

    glm::vec3 ToolPoint1, ToolPoint2;



    if (fabs(denominator - 1) <= 1e-6) // Вектор перпендикулярен плоскости цилиндра
    {
        intersectionPoint1 = MyMath::intersectRayToPlane(ray, near_plane);
        intersectionPoint2 = MyMath::intersectRayToPlane(ray, far_plane);

        float x = intersectionPoint1.x;
        float y = intersectionPoint1.y;

        if (x * x + y * y <= (D) * (D)) // Если вектор внутри окружности - записываем его
        {
            ToolPoint1 = intersectionPoint1;
            ToolPoint2 = intersectionPoint2;
        }

        else // Если вектор снаружи окружности - записываем на его место нулевой вектор
        {
            ToolPoint1 = glm::vec3(0, 0, 0);
            ToolPoint2 = glm::vec3(0, 0, 0);
        }
    }

    else if (fabs(denominator - 0) <= 1e-6) // Вектор параллелен плоскости цилиндра
    {
        if (ray.origin.z > 0 && ray.origin.z < H) //Если вектор попадает между плоскостей инструмента
        {
            if (MyMath::isQuadraticCanSolve(ray, D)) //Если вектор пересекает окружность - рассчитываем точку пересечения
            {
                std::pair<glm::vec3, glm::vec3> int_points = MyMath::intersectRayToSircle(ray, D);

                ToolPoint1 = int_points.first;
                ToolPoint2 = int_points.second;
            }

            else // Если вектор не пересекает окружность - записываем на его место нулевой вектор
            {
                ToolPoint1 = glm::vec3(0, 0, 0);
                ToolPoint2 = glm::vec3(0, 0, 0);
            }
        }
        else // Если вектор не попадает между плоскостей инструмента - записываем на его место нулевой вектор
        {
            ToolPoint1 = glm::vec3(0, 0, 0);
            ToolPoint2 = glm::vec3(0, 0, 0);
        }
    }

    else // Общий случай
    {
        if (MyMath::isQuadraticCanSolve(ray, D) == false)
        {
            ToolPoint1 = glm::vec3(0, 0, 0);
            ToolPoint2 = glm::vec3(0, 0, 0);
        }

        else
        {
            std::pair<glm::vec3, glm::vec3> int_points = MyMath::intersectRayToSircle(ray, D);

            ToolPoint1 = int_points.first;
            ToolPoint2 = int_points.second;

            if (ToolPoint1.z > ToolPoint2.z) // Сортируем точки по Z
            {
                glm::vec3 tmp = ToolPoint1;
                ToolPoint1 = ToolPoint2;
                ToolPoint2 = tmp;
            }

            Ray new_ray = { ToolPoint1,  ToolPoint2 - ToolPoint1 };

            if (ToolPoint1.z < 0 || ToolPoint1.z > H)
            {
                ToolPoint1 = MyMath::intersectRayToPlane(new_ray, near_plane);
            }

            if (ToolPoint2.z > H || ToolPoint2.z < 0)
            {
                ToolPoint2 = MyMath::intersectRayToPlane(new_ray, far_plane);
            }

            if (ToolPoint1.x * ToolPoint1.x + ToolPoint1.y * ToolPoint1.y > D * D)
            {
                if (ToolPoint2.x * ToolPoint2.x + ToolPoint2.y * ToolPoint2.y > D * D)
                {
                    ToolPoint1 = glm::vec3(0, 0, 0);
                    ToolPoint2 = glm::vec3(0, 0, 0);
                }

            }
        }
        

    }

    if (ToolPoint1.z > ToolPoint2.z) // Сортируем точки по Z
    {
        glm::vec3 tmp = ToolPoint1;
        ToolPoint1 = ToolPoint2;
        ToolPoint2 = tmp;
    }

    ToolPoint1 = transform(ToolPoint1);
    ToolPoint2 = transform(ToolPoint2);



    return glm::vec4(ToolPoint1, fabs(ToolPoint2.z - ToolPoint1.z) );

}



void DexelGrid::GenerateToolLines()
{
    tool_lines.clear();

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
        glm::vec3 point = transform(glm::vec3(tool_lines[i], tool_lines[i + 1], tool_lines[i + 2]));

        tool_lines[i] = point.x;
        tool_lines[i+1] = point.y;
        tool_lines[i+2] = point.z;
    }

    tool_line_offset.clear();

    tool_line_offset.push_back(transform(glm::vec3(0, 0, -offset)));
    tool_line_offset.push_back(transform(glm::vec3(0, 0, 0)));

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

        if (MyMath::Scalar_cyl(diam / 2, x, y))
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


void DexelGrid::GenerateToolGrid()
{

    if (tool_grid_list != nullptr)
    {
        delete[] tool_grid_list;
        tool_grid_list = nullptr;
    }

    tool_grid_list = new Dexel[X_size * Y_size];

    for (int i = 0; i < X_size * Y_size; i++)
    {
        float x = (i % X_size) - X_size / 2;
        float y = (i / X_size) % Y_size - Y_size / 2;

        glm::vec4 new_point = GetToolDexel((x - acc / 2) * acc, (y - acc / 2) * acc);

        tool_grid_list[i].start = round(new_point.z * 10) / 10 ;
        tool_grid_list[i].end = round((new_point.z + new_point.w) * 10) / 10;
        tool_grid_list[i].color = 1;

    }
}

void DexelGrid::GenerateDrawArrays()
{
    dexel_draw_data = new glm::vec4[summ_num_of_dexels + X_size*Y_size];
    colors_dexels = new float[summ_num_of_dexels + X_size * Y_size];

    tool_dexels.clear();


    int count = 0;

    for (int i = 0; i < X_size * Y_size; i++)
    {
        // Define Blank
        float x = (i % X_size) - X_size / 2;
        float y = (i / X_size) % Y_size - Y_size / 2;

        for (int num = 0; num < num_dexels[i]; num++)
        {
            float z = grid_list[i][num].start;
            float len = grid_list[i][num].end - grid_list[i][num].start;

            if (len != 0)
            {
                dexel_draw_data[count] = glm::vec4(x - acc/2, y - acc/2, z, len);  // Не забыть вернуть!!
                colors_dexels[count] = grid_list[i][num].color;
                count += 1;
            }
        }

    }

    for (int i = 0; i < X_size * Y_size; i++)
    {
        float x = (i % X_size) - X_size / 2;
        float y = (i / X_size) % Y_size - Y_size / 2;

        dexel_draw_data[count] = glm::vec4(x - acc / 2, y - acc / 2, tool_grid_list[i].start, tool_grid_list[i].end - tool_grid_list[i].start);
        colors_dexels[count] = tool_grid_list[i].color;
        count += 1;
        
    }

}


void DexelGrid::PaintBlankByTool(int color)
{

}


struct Coords
{
public:
    float x;
    float y;
    float z;
    float a;
    float c;

    static Coords CalcDetaMove(Coords old_c, Coords new_c)
    {
        Coords delta_coords = { new_c.x - old_c.x, new_c.y - old_c.y, new_c.z - old_c.z, new_c.a - old_c.a,new_c.c - old_c.c };
        return delta_coords;
    }

};


void DexelGrid::RunGProgramm(float t)
{
    Coords old_coords = { 100.0f, 40.0f, -35.0f, 45.0f, 180.0f };
    Coords new_coords = { 0.0f, 40.0f, -35.0f, 45.0f, -360.0f };
    
    Coords delta = Coords::CalcDetaMove(old_coords, new_coords);

    X = old_coords.x + delta.x * t;
    Y = old_coords.y + delta.y * t;
    Z = old_coords.z + delta.z * t;
    A = old_coords.a + delta.a * t;
    C = old_coords.c + delta.c * t;

}

void DexelGrid::RunGProgramm2(float t)
{
    Coords old_coords = { 100.0f, 40.0f, -35.0f, 45.0f, 360.0f };
    Coords new_coords = { 0.0f, 40.0f, -35.0f, 45.0f, -180.0f };

    Coords delta = Coords::CalcDetaMove(old_coords, new_coords);

    X = old_coords.x + delta.x * t;
    Y = old_coords.y + delta.y * t;
    Z = old_coords.z + delta.z * t;
    A = old_coords.a + delta.a * t;
    C = old_coords.c + delta.c * t;

}



void DexelGrid::BooleanOperation()
{
    for (int i = 0; i < X_size * Y_size; i++)
    {
        float tool_start = tool_grid_list[i].start;
        float tool_end = tool_grid_list[i].end;

        for (int num = 0; num < num_dexels[i]; num++)
        {
            float blank_start = grid_list[i][num].start;
            float blank_end = grid_list[i][num].end ;

            if (fabs(tool_start - tool_end) >= 1e-6  && fabs(blank_start - blank_end) >= 1e-6)
            {
                //         Tool
                //    --------------
                //           ---------------
                //                Blank
                if (tool_end >= blank_start &&
                    tool_start <= blank_start &&
                    tool_end <= blank_end)
                {
                    grid_list[i][num].start = tool_end;
                }

                //                  Tool
                //             --------------
                //    ---------------
                //         Blank
                if (tool_start > blank_start &&
                    tool_start < blank_end &&
                    tool_end > blank_end)
                {
                    grid_list[i][num].end = tool_start;
                }

                //             Tool
                //     --------------------
                //         ------------
                //             Blank
                if (tool_start < blank_start &&
                    tool_end > blank_end)
                {
                    std::vector<Dexel> tmp;
                    for (int j = 0; j < num_dexels[i]; j++)
                    {
                        if (j != num)
                        {
                            tmp.push_back(grid_list[i][j]);
                        }
                    }
                    delete[] grid_list[i];
                    grid_list[i] = nullptr;
                    num_dexels[i] -= 1;
                    summ_num_of_dexels -= 1;
                    grid_list[i] = new Dexel[num_dexels[i]];

                    for (int j = 0; j < num_dexels[i]; j++)
                    {
                        grid_list[i][j] = tmp[j];
                    }

                }


                //             Tool
                //         ------------
                //     ---------------------
                //             Blank
                if (tool_start > blank_start &&
                    tool_end < blank_end)
                {
                    std::vector<Dexel> tmp;
                    for (int j = 0; j < num_dexels[i]; j++)
                    {
                        if (j == num)
                        {
                            Dexel left;
                            left.start = blank_start;
                            left.end = tool_start;
                            left.color = 0;

                            Dexel right;
                            right.start = tool_end;
                            right.end = blank_end;
                            right.color = 0;

                            tmp.push_back(left);
                            tmp.push_back(right);
                        }
                        else
                        {
                            tmp.push_back(grid_list[i][j]);
                        }
                    }

                    delete[] grid_list[i];
                    grid_list[i] = nullptr;

                    int count = num_dexels[i];

                    num_dexels[i] += 1;
                    summ_num_of_dexels += 1;

                    grid_list[i] = new Dexel[num_dexels[i]];

                    for (int k = 0; k < num_dexels[i]; k++)
                    {
                        grid_list[i][k] = tmp[k];
                    }

                }
                
            }
        }

    }
}



void DexelGrid::DeleteDrawArrays()
{
    delete[] dexel_draw_data;
    dexel_draw_data = nullptr;

    delete[] colors_dexels;
    colors_dexels = nullptr;
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



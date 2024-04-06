#include "Tool.h"


void SetToolSettings(float D, float H)
{

}

void Tool::Generate_Tool(float D_, float H_, float resolution_, Coordinates coords, Blank* blank)
{
    Generate_Tool_lines(D_, H_, coords);
    Generate_Toool_Dexels(blank, coords);
}



void Tool::Generate_Tool_lines(float D_, float H_, Coordinates coords)
{
    D = D_/2;
    H = H_;

    coords.X *= -1;

    tool_lines.clear();

    float pi = 2 * asin(1.0);

    for (float angle = 0; angle < 2 * pi; angle += pi / 20)
    {
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, 0));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, H));

        tool_lines.push_back(glm::vec3(sinf(angle + pi / 20) * D, cosf(angle + pi / 20) * D, H));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, H));

        tool_lines.push_back(glm::vec3(sinf(angle + pi / 20) * D, cosf(angle + pi / 20) * D, 0));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, 0));

    }


    for (int i = 0; i < tool_lines.size(); i++)
    {
        tool_lines[i] = MyMath::transform(tool_lines[i], coords);
    }



    tool_offset_line.clear();

    tool_offset_line.push_back(MyMath::transform(glm::vec3(0, 0, -coords.offset), coords));
    tool_offset_line.push_back(MyMath::transform(glm::vec3(0, 0, 0), coords));
}

void Tool::Generate_Toool_Dexels(Blank* blank, Coordinates mashine_coords)
{

    tool_normals.clear();

    resolution = blank->resolution;

    X_grid_size = blank->X_grid_size;
    Y_grid_size = blank->Y_grid_size;


    tool_coords = mashine_coords;

    tool_coords.X *= -1;

    
    Clear_Arrays();

    Grid_size = X_grid_size * Y_grid_size;

    Grid = new Dexel*[Grid_size];
    //Grid = new std::vector<Dexel>[Grid_size];
    
    for (int i = 0; i < Grid_size; i++)
    {
        Grid[i] = new Dexel[2];
    }


    std::vector<int> iter;
    iter.resize(Grid_size);

    for (int i = 0; i < Grid_size; i++)
    {
        iter[i] = i;
    }

    std::for_each(std::execution::par, iter.begin(), iter.end(),
        [this](int i)
        {
            float x = (i % X_grid_size) - X_grid_size / 2;
            float y = (i / X_grid_size) % Y_grid_size - Y_grid_size / 2;

            ToolDexel dexel = GetToolDexel(x * resolution, y * resolution, tool_coords);

            Grid[i][0].start = dexel.start_point;
            Grid[i][0].end = dexel.start_point + dexel.len / 2 + 0.05;
            Grid[i][0].normal = dexel.start_normal;

            Grid[i][1].start = dexel.start_point + dexel.len / 2 - 0.05;
            Grid[i][1].end = dexel.start_point + dexel.len;
            Grid[i][1].normal = dexel.end_normal;
        });



    for (int i = 0; i < Grid_size; i++)
    {
        if (Grid[i][0].end - Grid[i][0].start < 1)
        {
            Grid[i][0].color = -1;          
            Grid[i][1].color = -1;
        }
        else
        {
            Grid[i][0].color = 1;
            Grid[i][1].color = 1;
            Num_of_Dexels += 2;
        }
    }  
}

void Tool::Clear_Arrays()
{
    if (Grid != nullptr)
    {
        for (int i = 0; i < Grid_size; i++)
        {
            delete[] Grid[i];
            Grid[i] = nullptr;
        }

        delete[] Grid;
        Grid = nullptr;
    }

    Num_of_Dexels = 0;
}




ToolDexel Tool::GetToolDexel(float dexel_x, float dexel_y, Coordinates coords)
{
    
    bool is_dexel_exists = false;

    ToolDexel dexel;
    dexel.start_normal = glm::vec3(1, 1, 1);
    dexel.end_normal = glm::vec3(1, 1, 1);



    glm::vec3 origin_0 = glm::vec3(dexel_x, dexel_y, 0.0f);
    glm::vec3 origin_1 = glm::vec3(dexel_x, dexel_y, 2 * H);

    glm::vec3 inv_origin_0 = MyMath::inv_transform(origin_0, coords);
    glm::vec3 inv_origin_1 = MyMath::inv_transform(origin_1, coords);

    glm::vec3 inv_origin_dir = inv_origin_1 - inv_origin_0;



    Ray ray = { inv_origin_0, glm::normalize(inv_origin_dir) };

    Plane near_plane = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
    Plane far_plane = { glm::vec3(0.0f, 0.0f, H), glm::vec3(0.0f, 0.0f, 1.0f) };

    float denominator = glm::dot(near_plane.normal, ray.direction); // –авен нулю если вектор параллелен плоскости

    glm::vec3 intersectionPoint1, intersectionPoint2;

    glm::vec3 ToolPoint1, ToolPoint2;



    if (fabs(denominator - 1) <= 1e-6) // ¬ектор перпендикул€рен плоскости цилиндра
    {
        intersectionPoint1 = MyMath::intersectRayToPlane(ray, near_plane);
        intersectionPoint2 = MyMath::intersectRayToPlane(ray, far_plane);

        float x = intersectionPoint1.x;
        float y = intersectionPoint1.y;

        if (x * x + y * y <= (D) * (D)) // ≈сли вектор внутри окружности - записываем его
        {
            ToolPoint1 = intersectionPoint1;
            ToolPoint2 = intersectionPoint2;

            is_dexel_exists = true;

            if (coords.A == 0 || coords.A == 180 || coords.A == -180)
            {
                dexel.start_normal = glm::vec3(0.f, 0.f, -1.f);
                dexel.end_normal = glm::vec3(0.f, 0.f, 1.f);
            }
            else
            {
                dexel.start_normal = glm::vec3(0.f, 0.f, 1.f);
                dexel.end_normal = glm::vec3(0.f, 0.f, -1.f);
            }



            if (x * x + y * y <= (D) * (D) && x * x + y * y >= 0.95 * (D) * (D))
            {
                dexel.start_normal = MyMath::transform(ToolPoint1 + glm::vec3(ToolPoint1.x, ToolPoint1.y, 0), coords) - MyMath::transform(ToolPoint1, coords);
                dexel.end_normal = MyMath::transform(ToolPoint1 + glm::vec3(ToolPoint1.x, ToolPoint1.y, 0), coords) - MyMath::transform(ToolPoint1, coords);
            }

            else
            {
                dexel.start_normal = MyMath::transform(ToolPoint1 + glm::vec3(0, 0, -1), coords) - MyMath::transform(ToolPoint1, coords);
                dexel.end_normal = MyMath::transform(ToolPoint2 + glm::vec3(0, 0, 1), coords) - MyMath::transform(ToolPoint2, coords);

            }


        }

        else // ≈сли вектор снаружи окружности - записываем на его место нулевой вектор
        {
            ToolPoint1 = glm::vec3(0, 0, 0);
            ToolPoint2 = glm::vec3(0, 0, 0);
        }

    }

    else if (fabs(denominator - 0) <= 1e-6) // ¬ектор параллелен плоскости цилиндра
    {
        if (ray.origin.z > 0 && ray.origin.z < H) //≈сли вектор попадает между плоскостей инструмента
        {
            if (MyMath::isQuadraticCanSolve(ray, D)) //≈сли вектор пересекает окружность - рассчитываем точку пересечени€
            {
                std::pair<glm::vec3, glm::vec3> int_points = MyMath::intersectRayToSircle(ray, D);

                ToolPoint1 = int_points.first;
                ToolPoint2 = int_points.second;

                is_dexel_exists = true;
            }

            else // ≈сли вектор не пересекает окружность - записываем на его место нулевой вектор
            {
                ToolPoint1 = glm::vec3(0, 0, 0);
                ToolPoint2 = glm::vec3(0, 0, 0);
            }
            
        }
        else // ≈сли вектор не попадает между плоскостей инструмента - записываем на его место нулевой вектор
        {
            ToolPoint1 = glm::vec3(0, 0, 0);
            ToolPoint2 = glm::vec3(0, 0, 0);
        }


        dexel.start_normal = ToolPoint1;
        dexel.end_normal = ToolPoint2;

    }

    else // ќбщий случай
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


            if (ToolPoint1.z > ToolPoint2.z) // —ортируем точки по Z
            {
                glm::vec3 tmp = ToolPoint1;
                ToolPoint1 = ToolPoint2;
                ToolPoint2 = tmp;
            }

            Ray new_ray = { ToolPoint1,  ToolPoint2 - ToolPoint1 };

            if (ToolPoint1.z < 0 || ToolPoint1.z > H)
            {
                ToolPoint1 = MyMath::intersectRayToPlane(new_ray, near_plane);
                dexel.start_normal = MyMath::transform(ToolPoint1 + glm::vec3(0, 0, -1), coords) - MyMath::transform(ToolPoint1, coords);
            }
            else
            {
                dexel.start_normal = MyMath::transform(ToolPoint1 + glm::vec3(ToolPoint1.x, ToolPoint1.y, 0), coords) - MyMath::transform(ToolPoint1, coords);
                dexel.end_normal = MyMath::transform(ToolPoint1 + glm::vec3(ToolPoint1.x, ToolPoint1.y, 0), coords) - MyMath::transform(ToolPoint1, coords);
            }

            if (ToolPoint2.z > H || ToolPoint2.z < 0)
            {
                ToolPoint2 = MyMath::intersectRayToPlane(new_ray, far_plane);
                dexel.end_normal = MyMath::transform(ToolPoint2 + glm::vec3(0, 0, 1), coords) - MyMath::transform(ToolPoint2, coords);
            }
            else
            {
                dexel.start_normal = MyMath::transform(ToolPoint1 + glm::vec3(ToolPoint1.x, ToolPoint1.y, 0), coords) - MyMath::transform(ToolPoint1, coords);
                dexel.end_normal = MyMath::transform(ToolPoint1 + glm::vec3(ToolPoint1.x, ToolPoint1.y, 0), coords) - MyMath::transform(ToolPoint1, coords);
            }

            if (ToolPoint1.x * ToolPoint1.x + ToolPoint1.y * ToolPoint1.y > D * D)
            {
                if (ToolPoint2.x * ToolPoint2.x + ToolPoint2.y * ToolPoint2.y > D * D)
                {
                    ToolPoint1 = glm::vec3(0, 0, 0);
                    ToolPoint2 = glm::vec3(0, 0, 0);
                }
                else
                {
                    is_dexel_exists = true;
                }

            }
            else
            {
                is_dexel_exists = true;
            }
        }


    }

    if (ToolPoint1.z > ToolPoint2.z) // —ортируем точки по Z
    {
        glm::vec3 tmp = ToolPoint1;
        ToolPoint1 = ToolPoint2;
        ToolPoint2 = tmp;
    }

    ToolPoint1 = MyMath::transform(ToolPoint1, coords);
    ToolPoint2 = MyMath::transform(ToolPoint2, coords);

    
    dexel.start_point = ToolPoint1.z;  
    dexel.len = fabs(ToolPoint2.z - ToolPoint1.z);


    if (is_dexel_exists == false)
    {
        dexel.start_point = 1000000;
        dexel.len = -1000000;
    }

    return dexel;
}

float Tool::Get_X_From_Grid_By_i(int i)
{
    return ((i % X_grid_size) - X_grid_size / 2) - resolution / 2;
}

float Tool::Get_Y_From_Grid_By_i(int i)
{
    return ((i / X_grid_size) % Y_grid_size - Y_grid_size / 2) - resolution / 2;
}

glm::vec4 Tool::Get_Dexel_To_Draw(int i, int j)
{
    return glm::vec4(Get_X_From_Grid_By_i(i), Get_Y_From_Grid_By_i(i), Grid[i][j].start, (Grid[i][j].end - Grid[i][j].start));
}
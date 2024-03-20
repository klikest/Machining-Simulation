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
    D = D_;
    H = H_;

    tool_lines.clear();

    float pi = 2 * asin(1.0);

    for (float angle = 0; angle < 2 * pi; angle += pi / 20)
    {
        tool_lines.push_back(glm::vec3( sinf(angle) * D, cosf(angle) * D, 0));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, H));

        tool_lines.push_back(glm::vec3(sinf(angle + pi / 20) * D, cosf(angle + pi / 20) * D, H));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, H));

        tool_lines.push_back(glm::vec3(sinf(angle + pi / 20) * D, cosf(angle + pi / 20) * D, 0));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, 0));
    }


    //MyMath::transformArray(tool_lines, coords);

    for (int i = 0; i < tool_lines.size(); i ++)
    {
        tool_lines[i] = MyMath::transform(tool_lines[i], coords);
    }

    tool_offset_line.clear();

    tool_offset_line.push_back(MyMath::transform(glm::vec3(0, 0, -coords.offset), coords));
    tool_offset_line.push_back(MyMath::transform(glm::vec3(0, 0, 0), coords));
}

void Tool::Generate_Toool_Dexels(Blank* blank, Coordinates mashine_coords)
{
	resolution = blank->resolution;

	X_grid_size = blank->X_grid_size;
	Y_grid_size = blank->Y_grid_size;

    Grid_size = X_grid_size * Y_grid_size;

    Clear_Arrays();

    Grid = new Dexel[Grid_size];

    for (int i = 0; i < Grid_size; i++)
    {
        float x = (i % X_grid_size) - X_grid_size / 2;
        float y = (i / X_grid_size) % Y_grid_size - Y_grid_size / 2;

        glm::vec4 new_point = GetToolDexel((x - resolution / 2) * resolution, (y - resolution / 2) * resolution, mashine_coords);

        Grid[i].start = round(new_point.z * 10) / 10;
        Grid[i].end = round((new_point.z + new_point.w) * 10) / 10;
        

        if (Grid[i].end - Grid[i].start < 1)
        {
            Grid[i].color = -1;
        }
        else
        {
            Grid[i].color = 1;
            Num_of_Dexels += 1;
        }

    }

}

void Tool::Clear_Arrays()
{
	if (Grid != nullptr)
	{
		delete[] Grid;
		Grid = nullptr;
	}

	Num_of_Dexels = 0;
}




glm::vec4 Tool::GetToolDexel(float dexel_x, float dexel_y, Coordinates coords)
{
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

    if (ToolPoint1.z > ToolPoint2.z) // —ортируем точки по Z
    {
        glm::vec3 tmp = ToolPoint1;
        ToolPoint1 = ToolPoint2;
        ToolPoint2 = tmp;
    }

    ToolPoint1 = MyMath::transform(ToolPoint1, coords);
    ToolPoint2 = MyMath::transform(ToolPoint2, coords);



    return glm::vec4(ToolPoint1, fabs(ToolPoint2.z - ToolPoint1.z));
}

float Tool::Get_X_From_Grid_By_i(int i)
{
    return ((i % X_grid_size) - X_grid_size / 2) - resolution / 2;
}

float Tool::Get_Y_From_Grid_By_i(int i)
{
    return ((i / X_grid_size) % Y_grid_size - Y_grid_size / 2) - resolution / 2;
}

glm::vec4 Tool::Get_Dexel_To_Draw(int i)
{
    return glm::vec4(Get_X_From_Grid_By_i(i), Get_Y_From_Grid_By_i(i), Grid[i].start, (Grid[i].end - Grid[i].start));
}
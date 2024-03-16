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





struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Plane
{
    glm::vec3 origin;
    glm::vec3 normal;
};



glm::vec3 intersectRayToPlane(Ray ray, Plane plane)
{
    float denominator = glm::dot(plane.normal, ray.direction);

    glm::vec3 pointToPlane = plane.origin - ray.origin;
    float t = glm::dot(pointToPlane, plane.normal) / denominator;

    return ray.origin + t * ray.direction;

}


glm::vec2 solveCuvadratic(float a, float b, float c)
{
    return glm::vec2(((-b+sqrt(b*b-4*a*c))/(2*a)), ((-b-sqrt(b*b-4*a*c))/(2*a)));
}


bool isQuadraticCanSolve(Ray ray, float d)
{
    float a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y;
    float b = 2 * (ray.direction.x * (ray.origin.x) + ray.direction.y * (ray.origin.y));
    float c = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y
        - (d ) * (d );

    float D = b*b - 4*a*c;
    if (D > 0) { return true; }
    else { return false; }
}

std::pair<glm::vec3, glm::vec3> intersectRayToSircle(Ray ray, float d)
{

    float a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y;
    float b = 2 * (ray.direction.x * (ray.origin.x) + ray.direction.y * (ray.origin.y));
    float c = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y
        - (d)*(d);

    glm::vec2 answer =  solveCuvadratic(a, b, c);

    glm::vec3 point1 = glm::vec3(ray.direction.x * answer.x + ray.origin.x,
        ray.direction.y * answer.x + ray.origin.y,
        ray.direction.z * answer.x + ray.origin.z);

    glm::vec3 point2 = glm::vec3(ray.direction.x * answer.y + ray.origin.x,
        ray.direction.y * answer.y + ray.origin.y,
        ray.direction.z * answer.y + ray.origin.z);

    std::pair<glm::vec3, glm::vec3> points = { point1, point2 };

    return points;
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
        intersectionPoint1 = intersectRayToPlane(ray, near_plane);
        intersectionPoint2 = intersectRayToPlane(ray, far_plane);

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
            if (isQuadraticCanSolve(ray, D)) //Если вектор пересекает окружность - рассчитываем точку пересечения
            {
                std::pair<glm::vec3, glm::vec3> int_points = intersectRayToSircle(ray, D);

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
        std::pair<glm::vec3, glm::vec3> int_points = intersectRayToSircle(ray, D);

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
            ToolPoint1 = intersectRayToPlane(new_ray, near_plane);
        }

        if (ToolPoint2.z > H || ToolPoint2.z < 0)
        {
            ToolPoint2 = intersectRayToPlane(new_ray, far_plane);
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

        glm::vec4 new_point = GetToolDexel((x - acc / 2) * acc, (y - acc / 2) * acc);

        dexel_draw_data[count] = glm::vec4(x - acc / 2, y - acc / 2, new_point.z, new_point.w);
        colors_dexels[count] = 2;
        count += 1;
    }

}



//void DexelGrid::BooleanOperation(Blank blank, Tool tool)
//{
//
//}



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



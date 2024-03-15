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



glm::vec3 intersectRayToPlane(glm::vec3 rayOrigin, glm::vec3 rayDirection,
    glm::vec3 planeNormal, glm::vec3 planePoint)
{
    float denominator = glm::dot(planeNormal, rayDirection);

    // Проверка, что луч не параллелен плоскости
    if (denominator != 0)
    {
        glm::vec3 pointToPlane = planePoint - rayOrigin;
        float t = glm::dot(pointToPlane, planeNormal) / denominator;

        // Проверка, что точка пересечения находится впереди луча
        if (t >= 0)
        {
            return rayOrigin + t * rayDirection;
        }
    }

    // В случае отсутствия пересечения
    return glm::vec3(std::numeric_limits<float>::infinity());
}



struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Cylinder
{
    glm::vec3 center;
    float radius;
    float height;
};

bool intersectRayCylinder(const Ray& ray, const Cylinder& cylinder, glm::vec3& intersectionPoint1, glm::vec3& intersectionPoint2)
{
    glm::vec3 a = glm::vec3(ray.direction.x, ray.direction.y, 0.0f);
    glm::vec3 o = glm::vec3(ray.origin.x, ray.origin.y, 0.0f);
    glm::vec3 c = glm::vec3(cylinder.center.x, cylinder.center.y, 0.0f);

    float A = glm::dot(a, a);
    float B = 2.0f * glm::dot(a, o - c);
    float C = glm::dot(o - c, o - c) - cylinder.radius * cylinder.radius;

    float discriminant = B * B - 4.0f * A * C;

    if (discriminant >= 0)
    {
        float t1 = (-B + sqrt(discriminant)) / (2.0f * A);
        float t2 = (-B - sqrt(discriminant)) / (2.0f * A);

        intersectionPoint1 = ray.origin + t1 * ray.direction;
        intersectionPoint2 = ray.origin + t2 * ray.direction;


        return true;
    }

    return false;
}


void DexelGrid::GenerateToolGrid()
{
    tool_dexels.clear();

    glm::vec3 origin_0 = glm::vec3(10, 10, 50);
    glm::vec3 origin_1 = glm::vec3(10, 10, 100);

    glm::vec3 inv_origin_0 = inv_transform(origin_0);
    glm::vec3 inv_origin_1 = inv_transform(origin_1);

    glm::vec3 inv_origin_dir = inv_origin_1 - inv_origin_0;


    glm::vec3 plane_normal = glm::vec3(0, 0, 1);
    glm::vec3 plane_near_point = glm::vec3(0, 0, 0);
    glm::vec3 plane_far_point = glm::vec3(0, 0, H);


    float denominator = glm::dot(plane_normal, inv_origin_dir);

    Ray ray = { inv_origin_0, inv_origin_dir };
    Cylinder cylinder = { glm::vec3(0.0f, 0.0f, 0.0f), D, -H };

    glm::vec3 intersectionPoint1, intersectionPoint2;
    glm::vec3 real_intersectionPoint1, real_intersectionPoint2;

    bool is_intersection_cyl = intersectRayCylinder(ray, cylinder, intersectionPoint1, intersectionPoint2);

    if (is_intersection_cyl)
    {
        if (intersectionPoint1.z > intersectionPoint2.z)
        {
            glm::vec3 tmp = intersectionPoint1;
            intersectionPoint1 = intersectionPoint2;
            intersectionPoint2 = tmp;
        }

        if (intersectionPoint1.z < 0 || intersectionPoint1.z > H)
        {
            intersectionPoint1 = intersectRayToPlane(intersectionPoint1, (intersectionPoint2 - intersectionPoint1), plane_normal, plane_near_point);
        }

        if (intersectionPoint2.z > H || intersectionPoint2.z < 0)
        {
            intersectionPoint2 = intersectRayToPlane(intersectionPoint1, (intersectionPoint2 - intersectionPoint1), plane_normal, plane_far_point);
        }


    }
    else
    {

        intersectionPoint1 = intersectRayToPlane(inv_origin_0, inv_origin_dir, plane_normal, plane_near_point);
        intersectionPoint2 = intersectRayToPlane(inv_origin_0, inv_origin_dir, plane_normal, plane_far_point);

        std::cout << intersectionPoint1.z << std::endl;
    }



    real_intersectionPoint1 = transform(intersectionPoint1);
    real_intersectionPoint2 = transform(intersectionPoint2);



    tool_dexels.push_back(intersectionPoint1);
    tool_dexels.push_back(intersectionPoint2);

    tool_dexels.push_back(real_intersectionPoint1);
    tool_dexels.push_back(real_intersectionPoint2);


}

void DexelGrid::GenerateToolLines()
{

    std::vector<float> my_tool;

    tool_lines.clear();

    my_tool.push_back(0);
    my_tool.push_back(0);
    my_tool.push_back(-offset);

    my_tool.push_back(0);
    my_tool.push_back(0);
    my_tool.push_back(H);


    float pi = 2 * asin(1.0);

    for (float angle = 0; angle < 2*pi; angle += pi / 20)
    {
        my_tool.push_back(sinf(angle)*D);
        my_tool.push_back(cosf(angle)*D);
        my_tool.push_back(0);

        my_tool.push_back(sinf(angle) * D);
        my_tool.push_back(cosf(angle) * D);
        my_tool.push_back(H);
        /////////////////
        my_tool.push_back(sinf(angle + pi / 20) * D);
        my_tool.push_back(cosf(angle + pi / 20) * D);
        my_tool.push_back(H);

        my_tool.push_back(sinf(angle) * D);
        my_tool.push_back(cosf(angle) * D);
        my_tool.push_back(H);
        //////////////////
        my_tool.push_back(sinf(angle + pi / 20) * D);
        my_tool.push_back(cosf(angle + pi / 20) * D);
        my_tool.push_back(0);

        my_tool.push_back(sinf(angle) * D);
        my_tool.push_back(cosf(angle) * D);
        my_tool.push_back(0);
    }


    for (int i = 0; i < my_tool.size(); i+=6)
    {
        glm::vec3 new_pos_ = inv_transform(glm::vec3(my_tool[i], my_tool[i+1], my_tool[i+2]));
        glm::vec3 new_pos2_ = inv_transform(glm::vec3(my_tool[i+3], my_tool[i + 4], my_tool[i + 5]));
        //tool_lines.push_back(new_pos_.x);
        //tool_lines.push_back(new_pos_.y);
        //tool_lines.push_back(new_pos_.z);
        //tool_lines.push_back(new_pos2_.x);
        //tool_lines.push_back(new_pos2_.y);
        //tool_lines.push_back(new_pos2_.z);

        glm::vec3 zero_pos_ = transform(new_pos_);
        tool_lines.push_back(zero_pos_.x);
        tool_lines.push_back(zero_pos_.y);
        tool_lines.push_back(zero_pos_.z);
        glm::vec3 zero_pos2_ = transform(new_pos2_);
        tool_lines.push_back(zero_pos2_.x);
        tool_lines.push_back(zero_pos2_.y);
        tool_lines.push_back(zero_pos2_.z);

        glm::vec3 last_pos_ = transform(zero_pos_);
        tool_lines.push_back(last_pos_.x);
        tool_lines.push_back(last_pos_.y);
        tool_lines.push_back(last_pos_.z);
        glm::vec3 last_pos2_ = transform(zero_pos2_);
        tool_lines.push_back(last_pos2_.x);
        tool_lines.push_back(last_pos2_.y);
        tool_lines.push_back(last_pos2_.z);
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
                dexel_draw_data[count] = glm::vec4(x - acc/2, y - acc/2, z, 0);  // Не забыть вернуть!!
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



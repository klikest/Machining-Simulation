#include "MyMath.h"





void MyMath::transformArray(std::vector<glm::vec3>& my_array, Coordinates coords)
{
    glm::mat4 rez_matrix = get_tranform_mat(coords);

    for (int i = 0; i < my_array.size(); i++)
    {
        glm::vec4 new_point = rez_matrix * glm::vec4(my_array[i], 1);
        my_array[i] = glm::vec3(new_point.x, new_point.y, new_point.z);
    }
}

void MyMath::inv_transformArray(std::vector<glm::vec3>& my_array, Coordinates coords)
{
    glm::mat4 rez_matrix = get_tranform_mat(coords);

    for (int i = 0; i < my_array.size(); i++)
    {
        glm::vec4 new_point = glm::inverse(rez_matrix) * glm::vec4(my_array[i], 1);
        my_array[i] = glm::vec3(new_point.x, new_point.y, new_point.z);
    }
}

void MyMath::transform_array_by_mat(std::vector<glm::vec3>& my_array, glm::mat4 matrix)
{
    for (int i = 0; i < my_array.size(); i++)
    {
        glm::vec4 new_point = matrix * glm::vec4(my_array[i], 1);
        my_array[i] = glm::vec3(new_point.x, new_point.y, new_point.z);
    }
}

glm::mat4 MyMath::get_tranform_mat(Coordinates coords)
{
    //glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 trans_1 = glm::translate(glm::mat4(1.0f), glm::vec3(coords.Y, coords.Z, coords.X));
    glm::mat4 trans_2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -coords.offset));
    glm::mat4 rot_2 = glm::rotate(glm::mat4(1.0f), glm::radians(coords.A), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 trans_3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, coords.offset));
    glm::mat4 rot_3 = glm::rotate(glm::mat4(1.0f), glm::radians(coords.C), glm::vec3(0.0, 0.0, 1.0));

    glm::mat4 rez_matrix = rot_3 * trans_3 * rot_2 * trans_2 * trans_1; //* rot;

    return rez_matrix;
}

glm::mat4 MyMath::get_blank_tranform_mat(Coordinates coords)
{
    glm::mat4 trans_1 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, coords.X));
    glm::mat4 trans_2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -coords.offset));
    glm::mat4 rot_2 = glm::rotate(glm::mat4(1.0f), glm::radians(coords.A), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 trans_3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, coords.offset));

    glm::mat4 rot_3 = glm::rotate(glm::mat4(1.0f), glm::radians(coords.C), glm::vec3(0.0, 0.0, 1.0));

    glm::mat4 rez_matrix = rot_3 * trans_3 * rot_2 * trans_2 * trans_1;

    return rez_matrix;
}


glm::vec3 MyMath::transform(glm::vec3 point, Coordinates coords)
{
    glm::vec4 Point = glm::vec4(point.x, point.y, point.z, 1.0f);

    glm::mat4 rez_matrix = get_tranform_mat(coords);

    Point = rez_matrix * Point;

    return glm::vec3(Point.x, Point.y, Point.z);
}

glm::vec3 MyMath::inv_transform(glm::vec3 point, Coordinates coords)
{
    glm::vec4 Point = glm::vec4(point.x, point.y, point.z, 1.0f);

    glm::mat4 rez_matrix = get_tranform_mat(coords);

    Point = glm::inverse(rez_matrix) * Point;

    return glm::vec3(Point.x, Point.y, Point.z);
}




int MyMath::Get_Grid_size(float grid_width, float resolution)
{
    if ((int)ceil(grid_width / resolution) % 2 == 1)
    {
        return ceil(grid_width / resolution) + 2;
    }
    else
    {
        return ceil(grid_width / resolution) + 3;
    }

    return 0;
}

bool MyMath::Scalar_cyl(float r, float x, float y)
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


glm::vec3 MyMath::intersectRayToPlane(Ray ray, Plane plane)
{
    float denominator = glm::dot(plane.normal, ray.direction);

    glm::vec3 pointToPlane = plane.origin - ray.origin;
    float t = glm::dot(pointToPlane, plane.normal) / denominator;

    return ray.origin + t * ray.direction;
}


glm::vec2 MyMath::solveCuvadratic(float a, float b, float c)
{
    return glm::vec2(((-b + sqrt(b * b - 4 * a * c)) / (2 * a)), ((-b - sqrt(b * b - 4 * a * c)) / (2 * a)));
}


bool MyMath::isQuadraticCanSolve(Ray ray, float d)
{
    float a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y;
    float b = 2 * (ray.direction.x * (ray.origin.x) + ray.direction.y * (ray.origin.y));
    float c = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y
        - (d) * (d);

    float D = b * b - 4 * a * c;
    if (D > 0) { return true; }
    else { return false; }
}

std::pair<glm::vec3, glm::vec3> MyMath::intersectRayToSircle(Ray ray, float d)
{

    float a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y;
    float b = 2 * (ray.direction.x * (ray.origin.x) + ray.direction.y * (ray.origin.y));
    float c = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y
        - (d) * (d);

    glm::vec2 answer = solveCuvadratic(a, b, c);

    glm::vec3 point1 = glm::vec3(ray.direction.x * answer.x + ray.origin.x,
        ray.direction.y * answer.x + ray.origin.y,
        ray.direction.z * answer.x + ray.origin.z);

    glm::vec3 point2 = glm::vec3(ray.direction.x * answer.y + ray.origin.x,
        ray.direction.y * answer.y + ray.origin.y,
        ray.direction.z * answer.y + ray.origin.z);

    std::pair<glm::vec3, glm::vec3> points = { point1, point2 };

    return points;
}



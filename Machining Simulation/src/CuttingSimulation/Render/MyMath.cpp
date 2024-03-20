#include "MyMath.h"





void MyMath::transformArray(std::vector<glm::vec3>& my_array, Coordinates coords)
{
    for (int i = 0; i < my_array.size(); i++)
    {
        my_array[i] = transform(my_array[i], coords);
    }
}

void MyMath::inv_transformArray(std::vector<glm::vec3>& my_array, Coordinates coords)
{
    for (int i = 0; i < my_array.size(); i++)
    {
        my_array[i] = inv_transform(my_array[i], coords);
    }
}

glm::vec3 MyMath::transform(glm::vec3 point, Coordinates coords)
{
    glm::vec4 pos_1 = glm::vec4(point.x, point.y, point.z + coords.offset, 0);

    glm::mat4 rotate_y = glm::mat4(1.0f);
    rotate_y = glm::rotate(rotate_y, glm::radians(coords.A), glm::vec3(0.0f, 1.0f, 0.0f)); // Матрица поворота вокруг оси А

    glm::vec4 pos_234 = (rotate_y * pos_1) + glm::vec4(coords.Z, coords.Y, -coords.offset + coords.X, 0);

    glm::mat4 rotate_x = glm::mat4(1.0f);
    rotate_x = glm::rotate(rotate_x, glm::radians(coords.C), glm::vec3(0.0f, 0.0f, 1.0f)); // Матрица поворота вокруг оси X

    glm::vec4 pos_5 = rotate_x * pos_234;

    return glm::vec3(pos_5.x, pos_5.y, pos_5.z);
}

glm::vec3 MyMath::inv_transform(glm::vec3 point, Coordinates coords)
{

    glm::mat4 rotate_x = glm::mat4(1.0f);
    rotate_x = glm::rotate(rotate_x, glm::radians(-coords.C), glm::vec3(0.0f, 0.0f, 1.0f)); // Матрица поворота вокруг оси X

    glm::vec4 pos_5 = rotate_x * glm::vec4(point.x, point.y, point.z, 1);
    glm::vec4 pos_43 = pos_5 - glm::vec4(coords.Z, coords.Y, coords.X - coords.offset, 0);

    glm::mat4 rotate_y = glm::mat4(1.0f);
    rotate_y = glm::rotate(rotate_y, glm::radians(-coords.A), glm::vec3(0.0f, 1.0f, 0.0f)); // Матрица поворота вокруг оси А

    glm::vec4 pos21 = (rotate_y * pos_43) - glm::vec4(0, 0, coords.offset, 0);

    return glm::vec3(pos21.x, pos21.y, pos21.z);

}




int MyMath::Get_Grid_size(float grid_width, float resolution)
{
    if ((int)ceil(grid_width / resolution) % 2 == 1)
    {
        return ceil(grid_width / resolution);
    }
    else
    {
        return ceil(grid_width / resolution) + 1;
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



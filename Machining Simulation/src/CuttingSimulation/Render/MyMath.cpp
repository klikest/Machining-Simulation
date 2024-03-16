#include "MyMath.h"


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



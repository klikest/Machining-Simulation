#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include "Coordinates.h"

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


class Coordinates;
class MyMath
{
public:
    static void transformArray(std::vector<glm::vec3>& my_array, Coordinates coords);
    static void inv_transformArray(std::vector<glm::vec3>& my_array, Coordinates coords);

    static glm::vec3 transform(glm::vec3 point, Coordinates coords);
    static glm::vec3 inv_transform(glm::vec3 point, Coordinates coords);

    static int Get_Grid_size(float grid_width, float resolution);
    static bool Scalar_cyl(float r, float x, float y);
    static glm::vec3 intersectRayToPlane(Ray ray, Plane plane);
    static glm::vec2 solveCuvadratic(float a, float b, float c);
    static bool isQuadraticCanSolve(Ray ray, float d);
    static std::pair<glm::vec3, glm::vec3> intersectRayToSircle(Ray ray, float d);
};


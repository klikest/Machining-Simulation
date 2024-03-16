#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 

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



class MyMath
{
public:
    static bool Scalar_cyl(float r, float x, float y);
    static glm::vec3 intersectRayToPlane(Ray ray, Plane plane);
    static glm::vec2 solveCuvadratic(float a, float b, float c);
    static bool isQuadraticCanSolve(Ray ray, float d);
    static std::pair<glm::vec3, glm::vec3> intersectRayToSircle(Ray ray, float d);
};


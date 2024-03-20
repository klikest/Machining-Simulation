#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include "Dexel.h"
#include "MyMath.h"

class Coordinates
{
public:

	float X = 0;
	float Y = 0;
	float Z = 0;
	float A = 0;
	float C = 0;
	float offset = 0;
};
#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Dexel.h"


class DexelGrid
{
public:
	int X_size;
	int Y_size;

	Dexel** Grid;

	glm::vec3 rect_min;
	glm::vec3 rect_max;
};


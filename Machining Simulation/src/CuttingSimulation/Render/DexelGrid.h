#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Dexel.h"


class DexelGrid
{
public:
	int X_size = 0;
	int Y_size = 0;
	int Z_size = 0;
	float acc = 1;

	Dexel* grid = nullptr;
	int* num_dexels = nullptr;
	int summ_num_of_dexels = 0;

	glm::vec4* dexel_draw_data = nullptr;
	float* colors_dexels = nullptr;

	glm::vec3 rect_min = glm::vec3(0,0,0);
	glm::vec3 rect_max = glm::vec3(0, 0, 0);
	DexelGrid();
	void CreateBlankCyl(float diam, float h, float acc);
	void GenerateDrawArrays();
	void DeleteArrays();
};


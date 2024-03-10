#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include "Dexel.h"


class DexelGrid
{
public:
	int X_size = 0;
	int Y_size = 0;
	int Z_size = 0;
	float acc = 1;


	float X = 65;
	float Y = 0;
	float Z = 0;
	float A = 15;
	float C = 0;

	float offset = 30;

	float D = 40;
	float H = 10;




	std::vector<float> tool_lines;

	Dexel** grid_list = nullptr;
	int* num_dexels = nullptr;
	int summ_num_of_dexels = 0;

	glm::vec4* dexel_draw_data = nullptr;
	float* colors_dexels = nullptr;

	glm::vec3 rect_min = glm::vec3(0,0,0);
	glm::vec3 rect_max = glm::vec3(0, 0, 0);

	DexelGrid();
	void GenerateToolLines();
	void CreateBlankCyl(float diam, float h, float acc);
	void GenerateDrawArrays();
	void DeleteArrays();
};


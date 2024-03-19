#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include "Dexel.h"
#include "MyMath.h"

class Blank
{
public:

	std::vector<Dexel>* Grid = nullptr;
	int Num_of_Dexels = 0;

	int Grid_size = 0;

	int X_grid_size = 0;
	int Y_grid_size = 0;

	float resolution = 0;
	float D = 0;
	float H = 0;


	void CreateCylBlank(float D, float H, float resolution);
	void ClearArrays();

	float Get_X_From_Grid_By_i(int i);

	float Get_Y_From_Grid_By_i(int i);

	glm::vec4 Get_Dexel_To_Draw(int i, int j);

};


#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include "Dexel.h"
#include "MyMath.h"
#include "Coordinates.h"

class Tool
{
public:

	Dexel* Grid = nullptr;
	int Num_of_Dexels = 0;

	std::vector<glm::vec3> tool_lines;
	std::vector<glm::vec3> tool_offset_line;

	int Grid_size = 0;

	int X_grid_size = 0;
	int Y_grid_size = 0;

	float resolution = 0;

	float D = 0;
	float H = 0;
	float offset = 30;

	void Generate_Tool_lines(float D, float H, Coordinates coords);
	void Generate_Toool_Dexels(float D_, float H_, float resolution, Coordinates mashine_coords);
	void Clear_Arrays();
};


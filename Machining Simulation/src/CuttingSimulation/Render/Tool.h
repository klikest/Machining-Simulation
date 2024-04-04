#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include <algorithm>
#include <execution>
#include "Dexel.h"
#include "MyMath.h"
#include "Coordinates.h"
#include "Blank.h"


struct ToolDexel
{
public:
	float start_point = 0;
	glm::vec3 start_normal = glm::vec3(0,0,0);

	float len = 0;
	glm::vec3 end_normal = glm::vec3(0, 0, 0);

	int color = -1;
};


class Tool
{
public:

	//Dexel* Grid = nullptr;
	std::vector<Dexel>* Grid = nullptr;
	int Num_of_Dexels = 0;

	Coordinates tool_coords;

	std::vector<glm::vec3> tool_lines;
	std::vector<glm::vec3> tool_offset_line;
	std::vector<glm::vec3> tool_normals;

	int Grid_size = 0;

	int X_grid_size = 0;
	int Y_grid_size = 0;

	float resolution = 0;

	float D = 0;
	float H = 0;
	float offset = 30;

	void Generate_Tool(float D_, float H_, float resolution_, Coordinates coords, Blank* blank);

	void Generate_Tool_lines(float D, float H, Coordinates coords);
	void Generate_Toool_Dexels(Blank* blank, Coordinates mashine_coords);
	void Clear_Arrays();

	ToolDexel GetToolDexel(float dexel_x, float dexel_y, Coordinates coords);

	float Get_X_From_Grid_By_i(int i);
	float Get_Y_From_Grid_By_i(int i);
	glm::vec4 Get_Dexel_To_Draw(int i, int j);
};


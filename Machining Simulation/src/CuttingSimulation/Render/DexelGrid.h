#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include "Dexel.h"
#include "MyMath.h"
#include "Blank.h"
#include "Tool.h"
#include "Coordinates.h"




class DexelGrid
{
public:
	float X = 80;
	float Y = 37;
	float Z = 0;
	float A = 15;
	float C = 0;


	float resolution = 0.5;

	float offset = 30;

	float D = 40;
	float H = 10;

	float blank_D = 20;
	float blank_H = 50;


	float tool_D = 60;
	float tool_H = 10;


	Blank* blank = nullptr;
	Tool* tool = nullptr;
	Coordinates machine_coords;


	glm::vec4* dexel_draw_data = nullptr;
	float* colors_dexels = nullptr;


	DexelGrid();
	void transformArray(std::vector<glm::vec3>& my_array);
	void inv_transformArray(std::vector<glm::vec3>& my_array);

	glm::vec3 transform(glm::vec3 point);
	glm::vec3 inv_transform(glm::vec3 point);

	void Generate_Draw_Arrays(Blank* blank, Tool* tool);
	void BooleanOperation(Blank* blank, Tool* tool);
	void DeleteDrawArrays();
};


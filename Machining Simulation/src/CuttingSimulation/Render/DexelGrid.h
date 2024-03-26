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
#include "Command.h"
#include "Profiler.h"




class DexelGrid
{
public:

	float resolution = 1;


	float blank_D = 20;
	float blank_H = 50;


	float tool_D = 60;
	float tool_H = 10;

	float len_of_cut = 0;


	Blank* blank = nullptr;
	Tool* tool = nullptr;
	Coordinates machine_coords;
	Command command;

	Profiler TimeData;

	glm::vec4* dexel_draw_data = nullptr;
	float* colors_dexels = nullptr;


	DexelGrid();

	void Generate_Draw_Arrays(Blank* blank, Tool* tool);
	void BooleanOperation(Blank* blank, Tool* tool);
	void DeleteDrawArrays();
};


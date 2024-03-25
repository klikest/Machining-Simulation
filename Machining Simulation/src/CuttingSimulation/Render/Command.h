#pragma once
#include <iostream> 
#include <fstream>
#include <string>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath> 
#include "Dexel.h"
#include "MyMath.h"
#include "Coordinates.h"


class Command
{
public:

	std::vector<Coordinates> command_list;


	float t = 0;
	int step = 0;

	void Parse_file(std::string path, Coordinates curret_coords);
	void Zero_time();
	void RunCommands(Coordinates& curret_coords, bool& run);

};
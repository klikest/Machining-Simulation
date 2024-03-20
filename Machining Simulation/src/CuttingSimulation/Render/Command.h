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
	
	static std::vector<Coordinates> command_list;

	static void Parse_file(std::string path, Coordinates curret_coords);
	static Coordinates RunCommands();

};
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Dexel.h"


class DexelScene
{
public:


	Dexel* blank_grid;

	int X_grid_size;
	int Y_grid_size;


	DexelScene();
	~DexelScene();

	void Init();

	void CreateCylBlank(float d, float h, float acc);

};


#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Dexel.h"
#include "DexelGrid.h"

class DexelScene
{
public:


	DexelGrid* Blank;
	DexelGrid* Tool;

	DexelScene();
	~DexelScene();

	void Init();

	void CreateCylBlank(float d, float h, float acc);

};


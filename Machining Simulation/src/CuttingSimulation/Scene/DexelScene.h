#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
class DexelScene
{
public:

	DexelScene();
	~DexelScene();

	void Init();
	void DrawCube();
};


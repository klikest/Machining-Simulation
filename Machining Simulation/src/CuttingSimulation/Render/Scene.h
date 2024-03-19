#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "DexelGrid.h"
#include <filesystem>
#include <vector>

class Scene
{
public:

	Shader shader;
	GLuint VAO = 0;
	GLuint VBO_main_dexel_vertices = 0;
	GLuint VBO_offsets = 0;
	GLuint VBO_colors = 0;

	
	DexelGrid* scene_grid;

	std::vector<float> offsets;
	std::vector<float> colors;
	std::vector<float> Cube_VertexPositions;

	Scene();

	void Init();

	void Draw();
	void Close();
};


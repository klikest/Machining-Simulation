#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "parse_stl.h"
#include <filesystem>
#include <vector>

class Mesh
{
public:


	Shader shader;
	GLuint VAO = 0;
	GLuint VBO_vertices = 0;
	GLuint VBO_colors = 0;

	std::vector<float> vertices;
	std::vector<float> colors;

	Mesh();

	void Init();

	void ReadMeshSTL(std::string stl_file_name);

	void DrawMesh();

};


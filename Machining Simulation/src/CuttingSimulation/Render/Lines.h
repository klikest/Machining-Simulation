#pragma once
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Camera.h"
#include <vector>

class Lines
{
public:

	Lines();

	Shader shader;
	GLuint VAO = 0;
	GLuint VBO_vertices = 0;
	GLuint VBO_colors = 0;

	std::vector<float> vertices;
	std::vector<float> colors;

	void Init();

	void AddLines(std::vector<float> new_vertices, std::vector<float> new_colors);
	void AddRectangle(glm::vec3 min_rect, glm::vec3 max_rect, glm::vec3 color);
	void AddCoords(glm::vec3 Pos);
	void Draw();
};


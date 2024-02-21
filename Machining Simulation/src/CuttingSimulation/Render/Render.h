#pragma once
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "FrameBuffer.h"
#include <vector>

class Render
{
public:
	float deltaTime = 0.0f;	
	float lastFrame = 0.0f;
	float currentFrame = 0.f;

	Shader line_shader;
	Shader dexel_shader;
	Camera camera;

	GLuint VAO_line = 0;
	GLuint VBO_vert_line = 0;
	GLuint VBO_color_line = 0;

	GLuint VAO_dexel = 0;
	GLuint VBO_vert_dexel = 0;
	GLuint VBO_color_dexel = 0;


	std::vector<float> line_vertices;
	std::vector<float> line_colors;


	Render();
	~Render();

	void Init(GLFWwindow* window);
	void AddCoords(glm::vec3 Pos);
	void AddLines(std::vector<float> vertices, std::vector<float> colors);
	void DrawLines();
	void Draw(GLFWwindow* window, float aspect);

private:
	void CalcDeltaTime();
};


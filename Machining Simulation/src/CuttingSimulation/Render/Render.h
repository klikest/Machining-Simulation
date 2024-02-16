#pragma once
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Camera.h"

class Render
{
public:
	float deltaTime = 0.0f;	
	float lastFrame = 0.0f;
	float currentFrame = 0.f;

	Shader line_shader;
	Camera camera;

	GLuint VAO_line = 0;
	GLuint VBO_vert_line = 0;
	GLuint VBO_color_line = 0;
	Render();
	~Render();

	void Init(GLFWwindow* window);
	void Draw(GLFWwindow* window);

private:
	void CalcDeltaTime();
};


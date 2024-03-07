#pragma once
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "FrameBuffer.h"
#include "Scene/DexelScene.h"
#include "Scene/parse_stl.h"
#include "Lines.h"
#include "GUI.h"
#include <vector>

class GUI;
class Render
{
public:
	float deltaTime = 0.0f;	
	float lastFrame = 0.0f;
	float currentFrame = 0.f;


	std::unique_ptr<Lines> lines;

	Camera camera;


	Shader line_shader;


	Shader dexel_shader;
	GLuint VAO_dexel = 0;
	GLuint VBO_vert_cube_for_dexels = 0;
	GLuint VBO_offsets_dexel = 0;
	GLuint VBO_color_dexel = 0;


	GLuint VAO_mesh = 0;
	GLuint VBO_vert_mesh = 0;
	GLuint VBO_color_mesh = 0;

	std::vector<float> mesh_vertices;
	std::vector<float> mesh_colors;

	std::vector<float> Cube_VertexPositions;



	std::string toolpath;


	Render();
	~Render();

	void Init(GLFWwindow* window);
	void ReadMesh(std::string stl_file_name);
	void DrawMesh(GUI* gui);
	void DrawScene(DexelScene* scene);
	void Draw(GLFWwindow* window, DexelScene* scene, float aspect, GUI* gui);

private:
	void CalcDeltaTime();
};


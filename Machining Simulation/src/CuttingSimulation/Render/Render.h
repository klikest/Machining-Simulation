#pragma once
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "FrameBuffer.h"
#include "DexelScene.h"
#include "parse_stl.h"
#include "Lines.h"
#include "Mesh.h"
#include "Scene.h"
#include "GUI.h"
#include <vector>
#include <time.h> 
#include <chrono>

class GUI;
class Render
{
public:
	float deltaTime = 0.0f;	
	float lastFrame = 0.0f;
	float currentFrame = 0.f;
	float render_time = 0.f;
	float global_render_time = 0.f;

	std::unique_ptr<Lines> lines;
	std::unique_ptr<Mesh> tool_mesh;
	std::unique_ptr<Scene> main_scene;

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


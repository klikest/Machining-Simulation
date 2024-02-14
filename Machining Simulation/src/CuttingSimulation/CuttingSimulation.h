#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm\gtc\type_ptr.hpp>

#include "Render/Render.h"


class CuttingSimulation
{
public:
	GLFWwindow* SimulationWindow;

	std::unique_ptr<Render> ptr_render_scene;

	int width;
	int height;
	std::string title;


	CuttingSimulation();

	void CreateSimWindow(int width_, int height_, std::string title_);
	void Run();
	void Close();
};

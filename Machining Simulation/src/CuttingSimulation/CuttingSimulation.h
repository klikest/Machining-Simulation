#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm\gtc\type_ptr.hpp>

#include "Render/Render.h"
#include "Render/GUI.h"
#include "Render/FrameBuffer.h"
#include "Render/DexelScene.h"


class CuttingSimulation
{
public:
	GLFWwindow* SimulationWindow;

	std::unique_ptr<Render> ptr_render_scene;
	std::unique_ptr<GUI> ptr_gui;
	std::unique_ptr<FrameBuffer> ptr_fbo;
	std::unique_ptr<DexelScene> ptr_dexel_scene;

	int width;
	int height;
	std::string title;


	CuttingSimulation();

	void CreateSimWindow(int width_, int height_, std::string title_);
	void Run();
	void Close();
};


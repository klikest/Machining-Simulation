#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "CuttingSimulation/CuttingSimulation.h"

class App
{
public:

	std::unique_ptr<CuttingSimulation> ptr_CuttingSimulationApp;

	void InitApp();
	void Run();
	void Close();
};

#pragma once

#include<GLFW/glfw3.h>
#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>

class GUI
{
public:
	GLFWwindow* curretWindow;

	void SetCurretWindow(GLFWwindow* simWindow);
	void Init();
	void Render();
	void Close();
};


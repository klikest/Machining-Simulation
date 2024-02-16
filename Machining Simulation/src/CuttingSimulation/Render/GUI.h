#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "FrameBuffer.h"

class GUI
{
public:
	GLFWwindow* curretWindow;

	float main_window_width = 0;
	float main_window_height = 0;

	void SetCurretWindow(GLFWwindow* simWindow);
	void Init();
	void ShowExampleAppDockSpace(bool* p_open);
	void Render(FrameBuffer fbo);
	~GUI();
};


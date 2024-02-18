#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "FrameBuffer.h"
#include "Render.h"

#include <vector>

class GUI
{
public:
	GLFWwindow* curretWindow;

	float main_window_width = 0;
	float main_window_height = 0;

	void SetCurretWindow(GLFWwindow* simWindow);
	void Init();
	void ShowExampleAppDockSpace(bool* p_open);
	void RenderMainScene(FrameBuffer fbo);
	void RenderSceneInfo(Render* render);
	void RenderGUI(FrameBuffer fbo, Render* render);
	GUI();
	~GUI();
};


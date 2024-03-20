#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imfilebrowser.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/implot.h>
#include "FrameBuffer.h"
#include "Render.h"
#include "Command.h"

#include <vector>
class Render;
class GUI
{
public:
	GLFWwindow* curretWindow;

	ImGui::FileBrowser fileDialog;

	float main_window_width = 0;
	float main_window_height = 0;

	float aspect = 0;


	float t = 0;

	std::vector<float> render_time;

	void add_data_to_plot(Render* render);

	void SetCurretWindow(GLFWwindow* simWindow);
	void Init();
	void ShowExampleAppDockSpace(bool* p_open);
	void RenderMainScene(FrameBuffer fbo);
	void RenderSceneInfo(Render* render);
	void RenderGUI(FrameBuffer fbo, Render* render);
	GUI();
	~GUI();
};


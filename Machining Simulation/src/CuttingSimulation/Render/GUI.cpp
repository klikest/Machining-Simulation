#include "GUI.h"


void GUI::SetCurretWindow(GLFWwindow* simWindow)
{
	curretWindow = simWindow;
}

void GUI::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(curretWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}



void GUI::Render()
{
	ImGuiIO& IO = ImGui::GetIO();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Test Frame!");
	ImGui::Text("This is some useful text.");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::Button("Button"))
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / IO.Framerate, IO.Framerate);
	ImGui::End();

	// Rendering
	ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(curretWindow, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//glClearColor(0.1, 0.1, 0.1, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GUI::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
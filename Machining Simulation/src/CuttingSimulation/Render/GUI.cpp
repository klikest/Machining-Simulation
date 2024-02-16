#include "GUI.h"


void GUI::SetCurretWindow(GLFWwindow* simWindow)
{
	curretWindow = simWindow;
}

void GUI::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(curretWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}



void GUI::Render(FrameBuffer fbo)
{
	ImGuiIO& IO = ImGui::GetIO();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ImGui::Begin("Scene");

	main_window_width = ImGui::GetContentRegionAvail().x;
	main_window_height = ImGui::GetContentRegionAvail().y;

	fbo.Rescale(main_window_width, main_window_height);


	glViewport(0, 0, main_window_width, main_window_height);

	// we get the screen position of the window
	ImVec2 pos = ImGui::GetCursorScreenPos();

	// and here we can add our created texture as image to ImGui
	// unfortunately we need to use the cast to void* or I didn't find another way tbh
	ImGui::GetWindowDrawList()->AddImage(
		(void*)fbo.texture_id,
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + main_window_width, pos.y + main_window_height),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();


	ImGui::ShowDemoWindow();

	// Rendering
	ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(curretWindow, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//glClearColor(0.1, 0.1, 0.1, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//GLFWwindow* backup_current_context = glfwGetCurrentContext();
	//ImGui::UpdatePlatformWindows();
	//ImGui::RenderPlatformWindowsDefault();
	//glfwMakeContextCurrent(backup_current_context);
}


void GUI::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
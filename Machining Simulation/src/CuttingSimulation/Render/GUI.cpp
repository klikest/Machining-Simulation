#include "GUI.h"



void GUI::add_data_to_plot(Render* render)
{
    if (render_time.size() < 500)
    {
        render_time.push_back(render->global_render_time);
    }
    else
    {
        render_time.erase(render_time.begin());
        render_time.push_back(render->global_render_time);
    }

}



void GUI::SetCurretWindow(GLFWwindow* simWindow)
{
	curretWindow = simWindow;
}

void GUI::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.ConfigViewportsNoAutoMerge = true;
	io.ConfigViewportsNoTaskBarIcon = true;

    //ImFont* font = io.Fonts->AddFontFromFileTTF("arial.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    //io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14.f,
    //    nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(curretWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init("#version 330");

    render_time.resize(500);


}

void GUI::ShowExampleAppDockSpace(bool* p_open)
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);


    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
            {

            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}


void GUI::RenderMainScene(FrameBuffer fbo)
{
    ImGui::Begin("Scene");

    main_window_width = ImGui::GetContentRegionAvail().x;
    main_window_height = ImGui::GetContentRegionAvail().y;

    aspect = main_window_width / main_window_height;

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
}

void GUI::RenderSceneInfo(Render* render)
{

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Info");


    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::SeparatorText("Coords");
    ImGui::SliderFloat("X", &render->main_scene->blank->X, -100.0f, 100.0f, "%.3f");
    ImGui::SliderFloat("Y", &render->main_scene->blank->Y, -100.0f, 100.0f, "%.3f");
    ImGui::SliderFloat("Z", &render->main_scene->blank->Z, -100.0f, 100.0f, "%.3f");
    ImGui::SliderFloat("A", &render->main_scene->blank->A, 0.0f, 90.0f, "%.3f");
    ImGui::SliderFloat("C", &render->main_scene->blank->C, -180.0f, 180.0f, "%.3f");

    
    static bool draw_tool_planes = true;
    ImGui::Checkbox("Draw tool planes", &draw_tool_planes);
    if (draw_tool_planes)
    {
        
    }



    static bool draw_tool = true;
    ImGui::Checkbox("Draw tool", &draw_tool);
    if (draw_tool)
    {
        render->main_scene->blank->GenerateToolLines();
        render->lines->AddLines(render->main_scene->blank->tool_lines, glm::vec3(1, 0.5, 1));
    }

    static bool draw_bbox = true;
    ImGui::Checkbox("Draw blank bbox", &draw_bbox);
    if (draw_bbox)
    {
        render->lines->AddRectangle(render->main_scene->blank->rect_min, render->main_scene->blank->rect_max, glm::vec3(0.5, 0.8, 0.2));
    }


    if (ImGui::CollapsingHeader("Debug info"))
    {
        ImGui::Text("Render Time %.3f ", render->render_time);
        ImGui::Text("Global render Time %.3f ", render->global_render_time);
        ImGui::Text("My FPS %.3f ", 1000 / render->global_render_time);
    }

    if (ImGui::CollapsingHeader("Blank") == false)
    {
        static float acc = 0.8f;
        static float diam = 16.0f;
        static float len = 60.0f;

        fileDialog.SetTitle("Select tool path");

        ImGui::SliderFloat("Grid size", &acc, 0.01f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diam", &diam, 4.0f, 20.0f, "%.3f");
        ImGui::SliderFloat("Len", &len, 10.0f, 150.0f, "%.3f");


        render->main_scene->blank->DeleteArrays();
        render->main_scene->blank->CreateBlankCyl(diam, len, acc);
        render->main_scene->blank->GenerateDrawArrays();

        render->main_scene->blank->GenerateToolGrid();
        render->lines->AddLines(render->main_scene->blank->tool_dexels, glm::vec3(0.8, 0.2, 0.4));
        

        if (ImGui::Button("Update blank"))
        {

        }

    }


    if (ImGui::CollapsingHeader("Tool"))
    {

        ImGui::SliderFloat("D", &render->main_scene->blank->D, 15.0f, 150.0f, "%.3f");
        ImGui::SliderFloat("H", &render->main_scene->blank->H, 8.0f, 30.0f, "%.3f");
        if (ImGui::Button("Update tool"))
        {

        }

    }


    if (ImGui::CollapsingHeader("Camera info"))
    {
        ImGui::Text("Fov = %.3f", render->camera.fov);
        ImGui::Text("Yaw = %.3f", render->camera.yaw);
        ImGui::Text("Pitch = %.3f", render->camera.pitch);
        ImGui::Text("X pos = %.3f", render->camera.cameraPos.x);
        ImGui::Text("Y pos = %.3f", render->camera.cameraPos.y);
        ImGui::Text("Z pos = %.3f", render->camera.cameraPos.z);
    }

    ImGui::End();


    ImGui::Begin("Plot Data");

    std::vector<float> x_data;
    std::vector<float> y_data;

    for (float i = 0; i < 500; i++)
    {
        x_data.push_back(i / 10);
        y_data.push_back( sinf(i/10) );
    }

    //ImGui::PlotLines("Some Plot", render_time.data(), render_time.size(), 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
    


    ImPlot::BeginSubplots("My Subplots", 1, 2, ImVec2(-1, -1), ImPlotSubplotFlags_NoTitle );
    
    ImPlot::SetNextAxesToFit();
    ImPlot::BeginPlot("Cut Force", ImVec2(-1, -1));
    ImPlot::PlotLine("Cut force plot", x_data.data(), y_data.data(), x_data.size());
    ImPlot::EndPlot();


    add_data_to_plot(render);
    ImPlot::SetNextAxesToFit();
    ImPlot::BeginPlot("Efficiency", ImVec2(-1, -1), ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit);
    ImPlot::PlotLine("Full render time, ms", x_data.data(), render_time.data(), x_data.size());
    ImPlot::EndPlot();


    ImPlot::EndSubplots();

    ImGui::End();
}


void GUI::RenderGUI(FrameBuffer fbo, Render* render)
{
	

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

    bool show = true;
    ShowExampleAppDockSpace(&show);

    
    RenderMainScene(fbo);
    RenderSceneInfo(render);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


GUI::GUI()
{

}


GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    ImPlot::DestroyContext();
}
#include "Render.h"



Render::Render()
{

}


Render::~Render()
{
}

void Render::Init(GLFWwindow* window)
{

    camera.Init(window, glm::vec3(-106.0f, 61.0f, 91.0f), -21, -26);


    lines = std::make_unique<Lines>();
    lines->Init();

    //tool_mesh = std::make_unique<Mesh>();
    //tool_mesh->Init();
    //tool_mesh->ReadMeshSTL("C://Users//User//source//repos//Machining Simulation//Machining Simulation//Models//Box1x1x1.stl");

    main_scene = std::make_unique<Scene>();
    main_scene->Init();

}



using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

void Render::Draw(GLFWwindow* window, float aspect, GUI* gui)
{

    CalcDeltaTime();
    camera.UpdateAspectRate(aspect);
    camera.MoveCamera(window, deltaTime);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    //camera.SetCamMatrixToShader(tool_mesh->shader.ID);
    //tool_mesh->DrawMesh();


    camera.SetCamMatrixToShader(main_scene->shader.ID);
    camera.SetCamPosToShader(main_scene->shader.ID);

    auto t0 = high_resolution_clock::now();

    main_scene->scene_grid->tool->Generate_Tool(main_scene->scene_grid->tool_D,
        main_scene->scene_grid->tool_H,
        main_scene->scene_grid->resolution,
        main_scene->scene_grid->machine_coords,
        main_scene->scene_grid->blank);

    lines->AddLines(main_scene->scene_grid->tool->tool_lines, glm::vec3(0.5, 0.4, 0.1));
    lines->AddLines(main_scene->scene_grid->tool->tool_offset_line, glm::vec3(0.5, 0.4, 0.1));
    lines->AddCoords(glm::vec3(0, 0, main_scene->scene_grid->blank_H));


    lines->AddLines({ glm::vec3(0, -20, 0), glm::vec3(0, 20, 0) }, glm::vec3(1, 1, 1));

    auto t1 = high_resolution_clock::now();
    duration<double, std::milli> tool_time_ms = t1 - t0;
    main_scene->scene_grid->TimeData.AddDataToArray(main_scene->scene_grid->TimeData.Generate_tool_time, tool_time_ms.count());


    main_scene->scene_grid->BooleanOperation(main_scene->scene_grid->blank, main_scene->scene_grid->tool);

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> bool_time_ms = t2 - t1;
    main_scene->scene_grid->TimeData.AddDataToArray(main_scene->scene_grid->TimeData.Boolean_op_time, bool_time_ms.count());


    main_scene->scene_grid->Generate_Draw_Arrays(main_scene->scene_grid->blank, main_scene->scene_grid->tool);


    auto t3 = high_resolution_clock::now();
    duration<double, std::milli> gen_draw_time_ms = t3 - t2;
    main_scene->scene_grid->TimeData.AddDataToArray(main_scene->scene_grid->TimeData.Generate_draw_array_time, gen_draw_time_ms.count());


    main_scene->Draw();


    auto t4 = high_resolution_clock::now();
    duration<double, std::milli> render_time_ms = t4 - t3;
    main_scene->scene_grid->TimeData.AddDataToArray(main_scene->scene_grid->TimeData.Render_scene_time, render_time_ms.count());



    camera.SetCamMatrixToShader(lines->shader.ID);
    //lines->AddCoords(glm::vec3(0, 0, 0));
    lines->Draw();

}

void Render::CalcDeltaTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

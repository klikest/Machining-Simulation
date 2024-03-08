#include "Render.h"



Render::Render()
{

}


Render::~Render()
{
}

void Render::Init(GLFWwindow* window)
{
    
    camera.Init(window, glm::vec3(-51.0f, 19.0f, 31.0f), -25, -13);


    lines = std::make_unique<Lines>();
    lines->Init();

    tool_mesh = std::make_unique<Mesh>();
    tool_mesh->Init();
    tool_mesh->ReadMeshSTL("C://Users//User//source//repos//Machining Simulation//Machining Simulation//Models//Box1x1x1.stl");

    main_scene = std::make_unique<Scene>();
    main_scene->Init();

}





void Render::Draw(GLFWwindow* window, DexelScene* scene, float aspect, GUI* gui)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();


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


    camera.SetCamMatrixToShader(lines->shader.ID);
    lines->AddRectangle(main_scene->blank->rect_min, main_scene->blank->rect_max, glm::vec3(0.5, 0.8, 0.2));
    lines->AddCoords(glm::vec3(0, 0, 0));
    lines->AddLines({ 0, 0, 0, 0, 25, 0 }, { 1, 1, 1, 1, 0.5, 0.5 });
    lines->Draw();


    //camera.SetCamMatrixToShader(tool_mesh->shader.ID);
    //tool_mesh->DrawMesh();
    
    camera.SetCamMatrixToShader(main_scene->shader.ID);
    camera.SetCamPosToShader(main_scene->shader.ID);
    main_scene->Draw();

   
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;

    render_time = ms_double.count();

    


}

void Render::CalcDeltaTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
